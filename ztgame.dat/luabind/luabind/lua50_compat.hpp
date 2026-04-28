// Lua 5.0 compatibility definitions
#ifndef LUA50_COMPAT_HPP
#define LUA50_COMPAT_HPP

// LUA_NOREF and LUA_REFNIL were introduced in Lua 5.1
// For Lua 5.0, we need to define them
#ifndef LUA_NOREF
#define LUA_NOREF (-2)
#endif

#ifndef LUA_REFNIL
#define LUA_REFNIL (-1)
#endif

#endif // LUA50_COMPAT_HPP
