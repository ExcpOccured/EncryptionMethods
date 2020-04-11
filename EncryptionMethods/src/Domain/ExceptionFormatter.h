#pragma once

#include <sstream>

namespace domain
{
	class Exception_formatter
	{
	public:
        Exception_formatter() = default;
        ~Exception_formatter() = default;

        template <typename Type>
        Exception_formatter& operator << (const Type& value)
        {
            stream_ << value;
            return *this;
        }

        std::string str() const { return stream_.str(); }
        explicit operator std::string() const { return stream_.str(); }

        enum ConvertToString
        {
            to_str
        };
		
        std::string operator >> (ConvertToString) const { return stream_.str(); }

    private:
        std::stringstream stream_;

        Exception_formatter(const Exception_formatter&);
		
        Exception_formatter& operator = (Exception_formatter&) = delete;
	};
}
