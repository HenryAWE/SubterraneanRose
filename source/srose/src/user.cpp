/**
 * @file user.cpp
 * @author HenryAWE
 * @brief User profile
 */

#include <sr/srose/user.hpp>
#include <iostream>
#include <sstream>
#include <boost/property_tree/info_parser.hpp>


namespace srose
{
    UserProfile::UserProfile(std::istream& is)
    {
        Load(is);
    }
    UserProfile::UserProfile(std::string name)
    {
        Create(std::move(name));
    }

    void UserProfile::Create(std::string name)
    {
        m_name.swap(name);
    }
    void UserProfile::Load(std::istream& is)
    {
        is >> std::ws;
        std::getline(is, m_name);
        std::int64_t size = 0;
        is >> size >> std::ws;
        std::string data;
        data.resize(size);
        is.read(data.data(), size);
        boost::property_tree::read_info(std::istringstream(data), m_data);
    }
    void UserProfile::Save(std::ostream& os)
    {
        os << m_name << std::endl;
        std::stringstream ss;
        boost::property_tree::write_info(ss, m_data);
        std::string data = ss.str();
        os << (std::uint64_t)data.size() << std::endl << data <<  std::endl;
    }

    std::string UserProfile::Get(const std::string& pt) const
    {
        return m_data.get<std::string>(pt, "");
    }
    void UserProfile::Set(const std::string& pt, std::string val)
    {
        m_data.put<std::string>(pt, std::move(val));
    }

    UserProfile::operator const std::string&() const noexcept
    {
        return m_name;
    }
} // namespace srose
