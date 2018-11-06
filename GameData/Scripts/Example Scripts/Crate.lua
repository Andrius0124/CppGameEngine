function load()
    print(position.x .. ", " .. position.y .. ", " .. position.z)
    print(rotation.x .. ", " .. rotation.y .. ", " .. rotation.z)
    print(scale.x    .. ", " .. scale.y    .. ", " .. scale.z   )
end

function update()

	rotation.x = rotation.x + 1
	rotation.y = rotation.y + 1
	rotation.z = rotation.z + 1

end

function keypress(key, pressed)
    if (pressed == true) then
		print("KEYDOWN - " .. key)
	else
		print("KEYUP - " .. key)

		if (key == "R") then
			reloadScene()
		else
			changeScene("game_state_1")
		end

	end
end