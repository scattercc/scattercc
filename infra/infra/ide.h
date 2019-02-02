#if !defined(_INFRA_IDE_H_INCLUDED_)
#define _INFRA_IDE_H_INCLUDED_

//
// Make sure INFRA_IDE_MODE is either not defined, or defined as true-like
//
#if defined(INFRA_IDE_MODE)
#   if !INFRA_IDE_MODE
#       error "INFRA_IDE_MODE is defined but not true-like. Don't do this! Either define it to true-like or undef it"
#   endif  // !INFRA_IDE_MODE
#endif  // defined(INFRA_IDE_MODE)


//
// If __INTELLISENSE__ is defined, we are likely in Visual Studio IDE
// If __RESHARPER__ is defined, we are likely in Visual Studio IDE (with Resharper C++)
//
#if defined(__INTELLISENSE__) || defined(__RESHARPER__)
#   if !defined(INFRA_IDE_MODE)
#       define INFRA_IDE_MODE 1
#   endif  // !defined(INFRA_IDE_MODE)
#endif  // defined(__INTELLISENSE__) || defined(__RESHARPER__)


#endif  // !defined(_INFRA_IDE_H_INCLUDED_)
