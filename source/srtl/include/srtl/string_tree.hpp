/**
 * @file string_tree.hpp
 * @author HenryAWE
 * @brief StringTree 
 */

#ifndef SRTL_string_tree_hpp_
#define SRTL_string_tree_hpp_

#include <stdexcept>
#include <map>
#include <optional>
#include <utility>
#include <algorithm>
#include <assert.h>
#include "string_tree_fwd.hpp"
#include "string_comparator.hpp"


namespace srtl
{
    /* Helper class */
    class string_tree_base
    {
    public:
        class path_not_found: public std::out_of_range
        {
        public:
            using out_of_range::out_of_range;
        };

    protected:
        [[noreturn]]
        static void throw_path_not_found()
        {
            throw path_not_found("path not found");
        }
    };

    /* Helper class */
    template <
        typename CharT,
        CharT Separator
    >
    class basic_string_path : public string_tree_base
    {
        std::basic_string_view<CharT> m_sv;
    public:
        typedef CharT char_type;
        typedef std::basic_string_view<CharT> string_view_type;
        typedef typename string_view_type::size_type size_type;

        basic_string_path() = default;
        basic_string_path(const basic_string_path& rhs) = default;
        basic_string_path(string_view_type sv)
            : m_sv(sv) {}

        class iterator
        {
            friend basic_string_path;
            string_view_type sv;
            std::size_t pos = 0, next_sep = string_view_type::npos;

            iterator(size_type pos_) noexcept : pos(pos_), next_sep(pos_) {};
            iterator(string_view_type sv_, std::size_t pos_ = 0) noexcept
                : sv(sv_), pos(pos_), next_sep(sv.find(Separator)) {}

            static constexpr CharT empty_string[1] = { CharT(0) };
        public:
            iterator() noexcept = default;
            iterator(const iterator& rhs) noexcept = default;

            bool operator==(const iterator& rhs) const noexcept
            {
                return pos==rhs.pos && next_sep==rhs.next_sep;
            }
            bool operator!=(const iterator& rhs) const noexcept
            {
                return !(*this == rhs);
            }
            iterator& operator++() noexcept
            {
                assert(pos != sv.npos);
                if (next_sep == sv.npos)
                {
                    pos = sv.npos;
                    return *this;
                }
                pos = next_sep + 1;
                next_sep = sv.find(Separator, next_sep + 1);
                return *this;
            }
            string_view_type operator*() const
            {
                if (pos == next_sep)
                    return empty_string;
                return sv.substr(pos, next_sep - pos);
            }
        };

        iterator begin() const
        {
            return iterator(m_sv);
        }
        iterator end() const
        {
            return iterator(m_sv.npos);
        }
    };

    template <
        typename T,
        typename CharT,
        CharT Separator
    >
    class basic_string_tree: public string_tree_base
    {
    public: 
        typedef T value_type;
        typedef std::optional<T> mapped_type;
        typedef basic_string_tree self_type;
        typedef std::basic_string<CharT> string_type;
        typedef std::basic_string_view<CharT> string_view_type;
        typedef basic_string_path<CharT, Separator> path_type;
        typedef string_comparator<CharT> comp;
        typedef std::map<string_type, self_type, comp> internal_data_type;
        typedef std::size_t size_type;
        typedef typename internal_data_type::iterator iterator;
        typedef typename internal_data_type::const_iterator const_iterator;

        static constexpr CharT separator() noexcept { return Separator; }
    
    private:
        internal_data_type m_children; // Child nodes
        mapped_type m_data; // Data stored by current node

    public:
        basic_string_tree() = default;
        basic_string_tree(const basic_string_tree& rhs)
            : m_children(rhs.m_children), m_data(rhs.m_data) {}
        basic_string_tree(basic_string_tree&& move) noexcept
            : m_children(std::move(move.m_children)), m_data(std::move(move.m_data)) {}
        basic_string_tree(value_type&& move)
            : m_children(), m_data(std::move(move)) {}
        basic_string_tree(const value_type& rhs)
            : m_children(), m_data(rhs) {}

        void swap(self_type& other) noexcept
        {
            m_data.swap(other.m_data);
            m_children.swap(other.m_children);
        }

        self_type& operator=(const self_type& rhs)
        {
            self_type(rhs).swap(*this);
            return *this;
        }
        self_type& operator=(self_type&& move) noexcept
        {
            self_type(std::move(move)).swap(*this);
            return *this;
        }

        template <typename... Args>
        value_type& emplace(Args&&... args)
        {
            return m_data.emplace(std::forward<Args>(args)...);
        }
        template <typename... Args>
        value_type& emplace_at(string_view_type pt, Args&&... args)
        {
            return force_path(pt)->emplace(std::forward<Args>(args)...);
        }
        void erase() noexcept
        {
            m_data.reset();
        }
        void erase_at(string_view_type pt) noexcept
        {
            auto iter = const_cast<self_type*>(walk_path(pt));
            if(iter)
                iter->erase();
        }

        /**
         * @brief Merge data from another string tree
         * 
         * @param other Another string tree
         */
        void merge(self_type& other)
        {
            m_children.merge(other.m_children);
            for(auto& i : other)
                m_children[i.first].merge(i.second);
            if(!m_data.has_value())
                m_data = std::move(other.m_data);
        }
        /**
         * @brief Merge data from another string tree
         * 
         * @param other Another string tree
         */
        void merge(self_type&& other)
        {
            m_children.merge(other.m_children);
            for(auto& i : other)
                m_children[i.first].merge(std::move(i.second));
            if(!m_data.has_value())
                m_data = std::move(other.m_data);
        }

        /**
         * @brief Clear data and child nodes
         */
        void clear() noexcept
        {
            m_data.reset();
            m_children.clear();
        }

        [[nodiscard]]
        /**
         * @brief Get the count of child nodes
         */
        size_type size() const noexcept { return m_children.size(); } 

        [[nodiscard]]
        bool has_value() const noexcept { return m_data.has_value(); }
        [[nodiscard]]
        bool has_value(string_view_type pt) const noexcept
        {
            auto iter = walk_path(pt);
            if(iter)
                return iter->has_value();
            else
                return false; // Not found, return false
        }

        [[nodiscard]]
        /**
         * @brief Get a child node, will create new node if path doesn't exist
         * 
         * @param pt Path
         * @return self_type& The child node
         */
        self_type& child(string_view_type pt)
        {
            return *force_path(pt);
        }

        [[nodiscard]]
        /**
         * @brief Get a child node
         * 
         * @param pt Path
         * @return std::optional<self_type> The new instance of the child node, or std::nullopt for empty node
         */
        std::optional<self_type> get_child_optional(string_view_type pt) noexcept
        {
            auto iter = walk_path(pt);
            if(iter)
                return std::optional<self_type>(*iter);
            else
                return std::nullopt;
        }

        [[nodiscard]]
        value_type get_value() const { return *m_data; }
        [[nodiscard]]
        value_type get_value(string_view_type pt) const
        {
            auto iter = walk_path(pt);
            if(iter)
                return *iter->m_data;
            else
                throw_path_not_found();
        }

        [[nodiscard]]
        std::optional<T> get_value_optional() const { return m_data; }
        [[nodiscard]]
        std::optional<T> get_value_optional(string_view_type pt)
        {
            auto iter = walk_path(pt);
            if(iter)
                return iter->m_data;
            else
                return std::nullopt;
        }

        /**
         * @brief Modify data of current node
         * 
         * @param func Unary function object
         */
        template <typename UnaryFunction>
        void modify(UnaryFunction func)
        {
            if(m_data.has_value())
                func(*m_data);
        }
        /**
         * @brief Modify the specific node
         * @remark Nothing will happen if the path cannot be found
         * 
         * @param pt Path
         * @param func Unary function object
         */
        template <typename UnaryFunction>
        void modify(string_view_type pt, UnaryFunction func)
        {
            auto iter = walk_path(pt);
            if(iter)
                const_cast<self_type*>(iter)->modify(std::move(func));
        }
        /**
         * @brief Access data of current node
         * 
         * @param func Unary function object
         */
        template <typename UnaryFunction>
        void access(UnaryFunction func) const
        {
            if(m_data.has_value())
                func(*m_data);
        }
        /**
         * @brief Access the specific node
         * @remark Nothing will happen if the path cannot be found
         * 
         * @param pt Path
         * @param func Unary function object
         */
        template <typename UnaryFunction>
        void access(string_view_type pt, UnaryFunction func) const
        {
            auto iter = walk_path(pt);
            if(iter)
                iter->access(std::move(func));
        }

        /* Iterators */
        iterator begin() noexcept { return m_children.begin();}
        iterator end() noexcept { return m_children.end(); }
        const_iterator begin() const noexcept { return m_children.begin();}
        const_iterator end() const noexcept { return m_children.end(); }
        const_iterator cbegin() const noexcept { return m_children.cbegin(); }
        const_iterator cend() const noexcept { return m_children.cend(); }

        /* Operators */
        value_type& operator[](string_view_type pt)
        {
            self_type* iter = force_path(pt);
            if(!iter->has_value())
                iter->m_data.emplace();
            return iter->m_data.value();
        }
        value_type& operator*() { return *m_data; }
        const value_type& operator*() const { return *m_data; }

    private:
        /**
         * @brief [Internal] Walk througth the path, a new child will be created when the path cannot be found
         * 
         * @param sv Path
         * @return self_type* Search result, never null
         */
        self_type* force_path(string_view_type sv)
        {
            path_type pt(sv);
            self_type* current = this;
            for (auto it = pt.begin(); it != pt.end(); ++it)
            {
                auto target = *it;
                current = &current->m_children[std::string(target)];
            }
            return current;
        }

        /**
         * @brief [Internal] Walk througth the path
         * 
         * @param sv Path
         * @return self_type const* Search result, or nullptr when the path cannot be found
         */
        self_type const* walk_path(string_view_type sv) const noexcept
        {
            path_type pt(sv);
            self_type const* current = this;

            for (auto it = pt.begin(); it != pt.end(); ++it)
            {
                auto target = *it;
                auto iter = current->m_children.find(string_view_type(target));
                if(iter == current->m_children.end())
                    return nullptr;    //Not found, return nullptr
                current = &iter->second;
            }
            return current;
        }
    };
} // namespace srtl


#endif
