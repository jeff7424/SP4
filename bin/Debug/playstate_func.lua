newT = 0
function timer (t, dt)
	newT= t + dt
end

newfps = 0
function calcfps(fps, fc, ti)
	fps = fc / (ti / 1000)
	newfps = fps
end

function keyboard(key)
	if key == 119 then
		moveMeForward(true, 0.1)
	end
	if key == 97 then
		moveMeSideway(true, 0.1)
	end
	if key == 115 then
		moveMeForward(false, 0.1)
	end
	if key == 100 then
		moveMeSideway(false, 0.1)
	end
end