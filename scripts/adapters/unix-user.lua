#!/usr/bin/env lua

local UnixUser = {}

-------------------------------------
-- Constructor method.
-------------------------------------
function UnixUser:new(client)
  local o = { client = client }
  setmetatable(o, self)
  self.__index = self
  return o
end

-------------------------------------
-- Tests the credentials.
-------------------------------------
function UnixUser:test_credentials(username, password)

  self.client:exec(string.format(
    'su %q 2> /dev/null << EOF\n'..
      '%s\n' ..
    'EOF',
    tostring(username),
    tostring(password)
  ))

  return self.client:get_return_code()
end

-------------------------------------
-- Changes the credentials.
-------------------------------------
function UnixUser:change_credentials(username, current_password, new_password)

  --[[
  self.client:exec(string.format(
    'echo %q | su %q',
    tostring(username),
    tostring(password)
  ))
  ]]

  return self.client:get_return_code()
end

return UnixUser