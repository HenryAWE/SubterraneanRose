/**
 * @file vfs.hpp
 * @author HenryAWE
 * @brief Virtual filesystem
 */

#ifndef SROSE_RES_vfs_hpp_
#define SROSE_RES_vfs_hpp_

#include <set>
#include <memory>
#include <string>
#include <vector>
#include <ios>
#include <sr/filesystem/filesystem.hpp>


namespace srose::vfs
{
    class VirtualFilesystem;

    class Path
    {
        std::string m_underlying;
        std::vector<std::string> m_cache; // Caching parsed data

        void AssignFromString(std::string value);

    public:
        typedef decltype(m_cache)::const_iterator iterator;
        typedef decltype(m_cache)::const_iterator const_iterator;

        Path(const Path& other) = default;
        Path(Path&& move) = default;
        Path(std::string pt) { AssignFromString(std::move(pt)); }
        Path(const char* pt)
            : Path(std::string(pt)) {}

        iterator begin() const { return m_cache.cbegin(); }
        iterator end() const { return m_cache.cend(); }

        Path Parent() const;
        std::string Filename() const;

        [[nodiscard]]
        constexpr const std::string& string() const noexcept { return m_underlying; }
        [[nodiscard]]
        constexpr operator const std::string&() const noexcept { return m_underlying; }
    };

    class VirtualFilesystem
    {
        /**
         * @brief Throw std::invalid_argument when pt is not a regular file
         *
         * @param pt Path of the file
         */
        static void CheckRegularFile(const filesystem::path& pt);
        /**
         * @brief Throw std::invalid_argument when pt does not exist
         *
         * @param pt Path of the file
         */
        static void CheckExistence(const filesystem::path& pt);

        /* Abstract file */
        struct FileBase
        {
            std::string name;
            FileBase* parent = nullptr;

            FileBase(FileBase* parent_, std::string name_)
                : parent(parent_), name(std::move(name_)) {}
            virtual ~FileBase() = default;

            virtual bool IsDirectory() const noexcept = 0;
            virtual bool IsRegularFile() const noexcept = 0;
            virtual std::size_t GetSize() const = 0;
        };
        /* Regular file */
        struct RegularFile : FileBase
        {
            using FileBase::FileBase;

            bool IsDirectory() const noexcept final { return false; }
            bool IsRegularFile() const noexcept final { return true; }
            virtual std::unique_ptr<std::istream> OpenRead() { throw std::invalid_argument(name + ": Write only"); }
            virtual std::unique_ptr<std::ostream> OpenWrite() { throw std::invalid_argument(name + ": Read only"); }
            virtual std::unique_ptr<std::iostream> OpenReadWrite() { throw std::invalid_argument(name + ": Unable to read/write"); }
        };
        struct Directory : FileBase
        {
            struct Comparator
            {
                typedef void is_transparent;

                static std::string_view Get(std::string_view sv) noexcept { return sv; }
                static std::string_view Get(const FileBase& f) noexcept { return f.name; }
                static std::string_view Get(const std::unique_ptr<FileBase>& pf) noexcept { return pf ? pf->name : std::string_view(); }

                template <typename T, typename U>
                bool operator()(T&& lhs, U&& rhs) const
                {
                    return Get(lhs) < Get(rhs);
                }
            };
            std::set<std::unique_ptr<FileBase>, Comparator> children;

            using FileBase::FileBase;

            bool IsDirectory() const noexcept final { return true; }
            bool IsRegularFile() const noexcept final { return false; }
            std::size_t GetSize() const final
            {
                std::size_t total = 0;
                for(auto& i : children)
                {
                    total += i ? 0 : i->GetSize();
                }

                return total;
            }

            template <typename F, typename... Args>
            F& AddChild(Args&&... args)
            {
                static_assert(std::is_base_of_v<FileBase, F>);
                auto& ret = **children.emplace(std::make_unique<F>(this, std::forward<Args>(args)...)).first;
                return static_cast<F&>(ret);
            }

            decltype(children)::iterator GetChild(std::string_view name)
            {
                return children.find(name);
            }
            decltype(children)::const_iterator GetChild(std::string_view name) const
            {
                return children.find(name);
            }
        };

        struct EmptyFile : RegularFile
        {
            using RegularFile::RegularFile;

            std::size_t GetSize() const override { return 0; }
        };
        struct NativeFile : RegularFile
        {
            filesystem::path native;
            std::ios_base::openmode mode;

            NativeFile(FileBase* parent, std::string name_, filesystem::path native_, std::ios_base::openmode mode_)
                : RegularFile(parent, std::move(name_)), mode(mode_)
            {
                CheckRegularFile(native_);
                native = std::move(native_);
            }

            std::size_t GetSize() const override;
            std::unique_ptr<std::istream> OpenRead() override;
            std::unique_ptr<std::ostream> OpenWrite() override;
            std::unique_ptr<std::iostream> OpenReadWrite() override;
        };

        Directory m_root;

        Directory* LocateDirectory(const Path& pt, bool force = true);
        FileBase* LocateFile(const Path& pt);

    public:
        VirtualFilesystem();

        ~VirtualFilesystem();

        void MountNativeFile(
            const Path& alias,
            const filesystem::path& native,
            std::ios::openmode mode = std::ios_base::in
        );

        std::unique_ptr<std::istream> Open(const Path& pt);
    };
} // namespace srose::vfs


#endif
