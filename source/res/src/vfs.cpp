/**
 * @file vfs.cpp
 * @author HenryAWE
 * @brief Virtual filesystem
 */

#include <sr/res/vfs.hpp>
#include <cassert>
#include <fstream>


namespace srose::vfs
{
    boost::char_separator<char> Path::sep("/");

    Path::Iterator::Iterator(const Path* pt, underlying_type iter)
        : m_pt(pt), m_iter(std::move(iter)) {}
    Path::Iterator& Path::Iterator::operator++()
    {
        ++m_iter;
        return *this;
    }

    Path Path::Parent() const
    {
        if(m_underlying.empty())
            return *this;
        auto index = m_underlying.rfind('/');
        return m_underlying.substr(0, index == 0 ? 1 : index);
    }
    std::string Path::Filename() const
    {
        auto index = m_underlying.rfind('/');
        if(index == m_underlying.npos || index + 1 > m_underlying.size())
            return std::string();
        return m_underlying.substr(index + 1);
    }

    void VirtualFilesystem::CheckRegularFile(const filesystem::path& pt)
    {
        if(!is_regular_file(pt))
            throw std::invalid_argument("is_regular_file(" + pt.u8string() + ") == false");
    }
    void VirtualFilesystem::CheckExistence(const filesystem::path& pt)
    {
        if(!exists(pt))
            throw std::invalid_argument("exists(" + pt.u8string() + ") == false");
    }

    std::size_t VirtualFilesystem::NativeFile::GetSize() const
    {
        return file_size(native);
    }
    std::unique_ptr<std::istream> VirtualFilesystem::NativeFile::OpenRead()
    {
        return std::make_unique<std::ifstream>(native, mode | std::ios_base::out);
    }
    std::unique_ptr<std::ostream> VirtualFilesystem::NativeFile::OpenWrite()
    {
        return std::make_unique<std::ofstream>(native, mode | std::ios_base::out);
    }
    std::unique_ptr<std::iostream> VirtualFilesystem::NativeFile::OpenReadWrite()
    {
        return std::make_unique<std::fstream>(native, mode | std::ios_base::in | std::ios_base::out);
    }

    VirtualFilesystem::Directory* VirtualFilesystem::LocateDirectory(const Path& pt, bool force)
    {
        Directory* iter = &m_root;
        for(auto& i : pt)
        {
            auto file = iter->GetChild(i);
            if(file == iter->children.end())
            {
                iter = force ? &iter->AddChild<Directory>(i) : nullptr;
            }
            else if(file->get()->IsDirectory())
            {
                iter = static_cast<Directory*>(file->get());
            }
            else
                return nullptr;
        }

        return iter;
    }
    VirtualFilesystem::FileBase* VirtualFilesystem::LocateFile(const Path& pt)
    {
        Directory* direct = LocateDirectory(pt.Parent(), false);
        if(!direct)
            return nullptr;
        auto iter = direct->GetChild(pt.Filename());
        return iter == direct->children.end() ? nullptr : iter->get();
    }

    VirtualFilesystem::VirtualFilesystem()
        : m_root(nullptr, "") {}

    VirtualFilesystem::~VirtualFilesystem() = default;

    void VirtualFilesystem::MountNativeFile(
            const Path& alias,
            const filesystem::path& native,
            std::ios::openmode mode
    ) {
        Directory* direct = LocateDirectory(alias.Parent(), true);
        if(!direct)
            throw std::invalid_argument("Invalid alias \"" + alias.string() + "\"");
        if(exists(native))
            CheckRegularFile(native);

        direct->AddChild<NativeFile>(alias.Filename(), native, mode);
    }

    std::unique_ptr<std::istream> VirtualFilesystem::Open(const Path& pt)
    {
        FileBase* file = LocateFile(pt);
        if(!file)
            throw std::invalid_argument("Exists(" + pt.string() + ") == false");
        if(!file->IsRegularFile())
            throw std::invalid_argument("IsRegularFile(" + pt.string() + ") == false");

        assert(dynamic_cast<RegularFile*>(file));
        return static_cast<RegularFile*>(file)->OpenRead();
    }
} // namespace srose::vfs
