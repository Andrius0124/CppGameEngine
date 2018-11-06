local tick = 0
local beatCounter = 0
local ticksPerBeat = 30
local pianoChannel = 0

function load()
	midiSetInstrument(pianoChannel, 0)
end

function update()
	if (tick >= ticksPerBeat) then
		tick = tick - ticksPerBeat
		
		midiPercussion(35, 100, 100)

		if (beatCounter == 0) then
			midiNote(pianoChannel, 60, 50, 1000 / 60 * (ticksPerBeat * 3 - 1))
			midiNote(pianoChannel, 64, 50, 1000 / 60 * (ticksPerBeat * 3 - 1))
			midiNote(pianoChannel, 67, 50, 1000 / 60 * (ticksPerBeat * 3 - 1))
		end

		beatCounter = beatCounter + 1
		if (beatCounter == 4) then
			beatCounter = 0
		end
	end
	tick = tick + 1
end