#!/usr/bin/env lua

local LocalClient = {}

-------------------------------------
-- Constructor method.
-------------------------------------
function LocalClient:new()
  local o = { return_code = 0 }
  setmetatable(o, self)
  self.__index = self
  return o
end

-------------------------------------
-- Executes a command.
-------------------------------------
function LocalClient:exec(command)
  local handle, result

  handle = io.popen(command)
  result = handle:read('*a')
  self.return_code = handle:close()

  return result
end

-------------------------------------
-- Returns the command exit code.
-------------------------------------
function LocalClient:get_return_code()
  return self.return_code
end

return LocalClient
