local groundHeight = 0.8

local gravity = 0.003
local friction = 0.990
local bounciness = 0.80

local pitchWidth = 17
local pitchHeight = 23
local shootSpeed = 0.25
local pianoChannel = 0

local hits = 0

function load()
	midiSetInstrument(pianoChannel, 0)
end

function update()


	velocity.y = velocity.y - gravity

	local player = getEntity("player")
	local xdist = player.position.x - position.x
	local ydist = (player.position.y + 0.33) - position.y
	local zdist = player.position.z - position.z

	local distanceToBall = sqrt(xdist * xdist + ydist * ydist + zdist * zdist)
	if (distanceToBall < 1.5) then
		-- Normalize the vector
		xdist = xdist / distanceToBall
		zdist = zdist / distanceToBall

		velocity.x = -xdist * shootSpeed
		velocity.z = -zdist * shootSpeed
		velocity.y = shootSpeed / 4
		
		midiPercussion(35, 35, 35)
		hits = hits + 1
		setText("score", "Hits: " .. hits)

		
	end

	position.x = position.x + velocity.x * deltaTime
	position.y = position.y + velocity.y * deltaTime
	position.z = position.z + velocity.z * deltaTime
	
	if (position.y < groundHeight) then
		position.y = groundHeight
		velocity.y = -velocity.y * bounciness
	end

	if (position.x < -pitchWidth) then
		position.x = -pitchWidth
		velocity.x = -velocity.x * bounciness
		midiPercussion(35, 35, 35)
	elseif (position.x > pitchWidth) then
		position.x = pitchWidth
		velocity.x = -velocity.x * bounciness
		midiPercussion(35, 35, 35)
	end

	if (position.z < -pitchHeight) then
		position.z = -pitchHeight
		velocity.z = -velocity.z * bounciness
		midiPercussion(35, 35, 35)
	elseif (position.z > pitchHeight) then
		position.z = pitchHeight
		velocity.z = -velocity.z * bounciness
		midiPercussion(35, 35, 35)
	end

	velocity.x = velocity.x * friction
	velocity.z = velocity.z * friction
end

function keypress(key, pressed)

end