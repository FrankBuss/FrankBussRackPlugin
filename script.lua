-- echo delay in seconds
delay = 0.3

-- echo damping factor
damping = 0.9

addLabel(50, 123, "-> Echo ->", 20)
inp = addInput(20, 120)
out = addOutput(170, 120)
lastSampleRate = 0

function initBuffer()
	-- create a buffer for the echo and init it to 0
	length = math.floor(sampleRate * delay) * 2
	buffer = {}
	for i=0,length-1 do buffer[i] = 0 end
	
	-- init ringbuffer index
	i1 = 0
	i2 = length / 2
	
	-- save last sample rate
	lastSampleRate = sampleRate

	print("echo buffer initializd, length: " .. length .. " samples")
end

function process()
	-- init buffer on sample rate changes and on start
	if lastSampleRate ~= sampleRate then
		initBuffer()
	end
	
	-- get input voltage
	v = getVoltage(inp)
	
	-- add with delayed previous value
	buffer[i1] = buffer[i2] * damping + v
	
	-- set output voltage
	setVoltage(out, buffer[i1])
	
	-- increment ringbuffer index
	i1 = i1 + 1
	if i1 == length then
		i1 = 0
	end
	i2 = i2 + 1
	if i2 == length then
		i2 = 0
	end
end
