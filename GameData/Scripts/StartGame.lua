function load()
end

function update()
end

function keypress(key, pressed)
	if (pressed) then
		setText("instructions", "LOADING...")
	else
		changeScene("gameplay")
	end
end