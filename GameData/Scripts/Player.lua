local forward  = false
local backward = false
local left     = false
local right    = false

local movespeed = 0.01
local deceleration = 0.95;
local bounciness = 0.85

local pitchWidth = 17
local pitchHeight = 22

function load() 
	captureMouse(captureEnable)
end

function update()
	velocity.x = velocity.x * deceleration
	velocity.y = velocity.y * deceleration
	velocity.z = velocity.z * deceleration

	if (forward) then
		velocity.x = velocity.x + movespeed * sin(rotation.y)
		velocity.z = velocity.z + movespeed * cos(rotation.y)
	end

	if (backward) then
		velocity.x = velocity.x - movespeed * sin(rotation.y)
		velocity.z = velocity.z - movespeed * cos(rotation.y)
	end

	if (left) then
		velocity.x = velocity.x + movespeed * sin(rotation.y + 90)
		velocity.z = velocity.z + movespeed * cos(rotation.y + 90)
	end

	if (right) then
		velocity.x = velocity.x + movespeed * sin(rotation.y - 90)
		velocity.z = velocity.z + movespeed * cos(rotation.y - 90)
	end

	-- Move by the current speed relative to the framerate
	position.x = position.x + velocity.x * deltaTime
	position.y = position.y + velocity.y * deltaTime
	position.z = position.z + velocity.z * deltaTime

	if (position.x < -pitchWidth) then
		position.x = -pitchWidth
		velocity.x = 0
	elseif (position.x > pitchWidth) then
		position.x = pitchWidth
		velocity.x = 0
	end

	if (position.z < -pitchHeight) then
		position.z = -pitchHeight
		velocity.z = 0
	elseif (position.z > pitchHeight) then
		position.z = pitchHeight
		velocity.z = 0
	end

	rotation.y = getEntity("camera").rotation.y
end

function keypress(key, pressed)
	-- Save the state of these keys when they change
	if (key == "W") then forward  = pressed end
	if (key == "S") then backward = pressed end
	if (key == "A") then left     = pressed end
	if (key == "D") then right    = pressed end
	if (key == "R" and pressed) then reloadScene() end
end