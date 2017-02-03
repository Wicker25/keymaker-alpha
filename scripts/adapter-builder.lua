#!/usr/bin/env lua

-------------------------------------
-- Builds the adapter.
-- @param options The parameters.
-------------------------------------
local function build_adapter(client, options)

  local type = tostring(options.type)

  local Adapter = require('adapters/' .. type)
  local adapter = Adapter:new(client)

  local r = adapter:test_credentials(options.username, options.password)

  if r ~= 0
  then
    io.stderr:write('Test fail!\n')
  else
    print('Passed!')
  end
end

-- Export the builder function
core.build_adapter = build_adapter