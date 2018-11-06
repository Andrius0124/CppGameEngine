function load()
    -- Do nothing
end

function update()
	-- Do nothing
end

function keypress(key, pressed)
	local channel    = 0   -- The channel to use
	local instrument = 0   -- Piano
	local pitch      = 60  -- Middle C
	local velocity   = 100 -- Full power
	local duration   = 100 -- Duration of sound

	-- Set the instrument for this channel
	--midiSetInstrument(channel, instrument)

    if (pressed == true) then

		if (key == "Space") then
			midiPercussion(35, 100, 100)
		else
	
			if (key == "1") then
				pitch = 36;
			elseif (key == "2") then
				pitch = 48;
			elseif (key == "3") then
				pitch = 60;
			elseif (key == "4") then
				pitch = 72;
			elseif (key == "5") then
				pitch = 84;
			end

			-- Play a note on this channel
			midiNote(channel, pitch, velocity, duration)
		end

	end
end