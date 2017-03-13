#ifndef _PROC_SHELL_H
#define _PROC_SHELL_H

#include "Shell.h"
#include <map>

///
/// \brief An implementation of a Shell that executes procedures
///  (take-nothing, return-nothing) functions
///
/// \authors Yann Sanquer
///
/// This Shell implementation simply maps commands to their callbacks
///  and matches the commands against a word from the input stream
///
class ProcShell : public Shell< std::function<void()> >
{
    public:
        ///
        /// \brief Procedure
        ///
        typedef std::function<void()> Proc;

    private:
        std::map< std::string, std::pair< Proc, std::string > > _command_map;

    public:
        ///
        /// \brief Constructor
        ///
        ProcShell();

        ///
        /// \brief Destructor
        ///
        virtual ~ProcShell();

        virtual ProcShell &when(std::string const &command, Proc callback, std::string const &explain) override;

        virtual std::string explain(std::string const &command) const override;

    private:
        virtual void parse_istream(std::istream &) override;
};

#endif
