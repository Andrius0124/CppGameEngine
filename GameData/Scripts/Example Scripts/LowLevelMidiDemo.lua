function load()
    -- Do nothing
end

function update()
	-- Do nothing
end

function keypress(key, pressed)
	local piano_channel = 0   -- The channel to use
	local instrument    = 0   -- Piano
	local pitch         = 60  -- Middle C
	local velocity      = 100 -- Full power

    if (pressed == true) then
		-- Set the instrument for this channel
		midiSetInstrument(piano_channel, instrument);

		-- Play a note on this channel
		midiOn(piano_channel, pitch, velocity);
	else
		-- Release the note
		midiOff(piano_channel, pitch);
	end
end