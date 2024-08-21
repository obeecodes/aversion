
-- called every frame
function update()
    local id = entityID
    local vec = {x = playerX + 8  - pupX, y = playerY + 8 - pupY}

    local x = vec.x
    local y = vec.y

    if x ~= 0 and y ~= 0 then
        if math.abs(x) * 8 > math.abs(y) * 8 then 
            y = y --0
        else 
            x = x --0
        end
    end

    --[[if vec.x^2 > vec.y^2 then 
        if vec.x > 0 then
            pupX = pupX + 1
        else 
            pupX = pupX - 1
        end

    else 
        if vec.y > 0 then
            pupY = pupY + 1
        else 
            pupY = pupY - 1
        end
    end ]]


    if math.abs(vec.x) < 32 and math.abs(vec.y) < 32 then 
        x = 0
        y = 0
    end

    --print("X vel: " .. x)
    --print("Y vel: " .. y)
    print("Vec2: <" .. vec.x .. ", " .. vec.y .. ">")
    --print("X: " .. pupX)
    --print("Y: " .. pupY)
    --print(string.format("X: %d, Y: %d", x, y))

    setEntityVelocity(id, x, y) 
end 

-- self.sprite = "assets/images/puppy.atlas"
-- physics
-- transform
-- collider
-- audio

update()

