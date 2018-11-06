local score = 0
local pianoChannel = 0

function load()
	midiSetInstrument(pianoChannel, 0)
end

function update()
--[[
	setText("score", "Hits: " .. score)
	
	local goalframe = getEntity("goal_netting_2")
	local ball = getEntity("ball")
	
	local xdist = goalframe.position.x - ball.position.x
	local ydist = goalframe.position.y - ball.position.y
	local zdist = goalframe.position.z - ball.position.z
	
	local distance = sqrt(xdist * xdist + ydist * ydist + zdist * zdist)
	if (distance < 1.5) then

		midiPercussion(35, 35, 35)
		score = score + 1
	end	
	]]--

end