local forward  = false
local backward = false
local left     = false
local right    = false
local up       = false
local down     = false

local xspeed = 0
local yspeed = 0
local zspeed = 0
local movespeed = 0.3

local captureEnable = true

function load() 
	captureMouse(captureEnable)
end

function update()
	
	local playerEntity = getEntity("player")

	position.x = playerEntity.position.x
	position.y = playerEntity.position.y + 1.5
	position.z = playerEntity.position.z

	-- Turn the camera based on mouse movement
	if (captureEnable) then
		rotation.x = rotation.x - mouse.y * 0.1 * deltaTime
		rotation.y = rotation.y - mouse.x * 0.1 * deltaTime
		if (rotation.x < -89) then rotation.x = -89 end
		if (rotation.x >  89) then rotation.x =  89 end
	end
end

function keypress(key, pressed)
	if (pressed) then
		-- Toggle capturing mouse movement
		if (key == "Escape") then
			captureEnable = not captureEnable
			captureMouse(captureEnable)
		end
	end
end