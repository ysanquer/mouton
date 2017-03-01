#ifndef _SHELL_H
#define _SHELL_H

#include <iostream>
#include <string>
#include <functional>
#include <exception>

///
/// \brief An object that maps character strings (commands) to functions
///
/// These objects can match commands against an input character stream
///  and execute the corresponding functions
///
template<typename Callback>
class Shell
{
        ///
        /// \brief Stream extraction operator
        ///
        /// Extracts a command from the input stream and calls the
        ///  corresponding callback
        ///
        template<typename _Callback>
		friend std::istream &operator>>(std::istream &, Shell<_Callback> &);

	public:
        ///
        /// \brief Destructor
        ///
		virtual ~Shell();

        ///
        /// \brief Maps a command to a callback
        ///
		virtual Shell &when(std::string const &command, Callback callback, std::string const &explain) = 0;

        ///
        /// \brief Explain the shell's commands
        ///
        virtual std::string explain(std::string const &) const = 0;

	private:
        ///
        /// \brief Parse input character stream
        ///
		virtual void parse_istream(std::istream &) = 0;
};

template<typename Callback>
inline Shell<Callback>::~Shell()
{
}

template<typename Callback>
std::istream &operator>>(std::istream &in, Shell<Callback> &sh)
{
    sh.parse_istream(in);
	return in;
}

#endif
