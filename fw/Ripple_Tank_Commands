# User interface -> Firmware
{"set":"start"}			# starts loudspeaker vibrations
{"set":"stop"}			# stops loudspeaker vibrations
{"set":"pulse", "to": 2}	# pulse loudspeaker for param number of seconds
{"set":"frequency", "to":10}	# command sends a percentage value 0-100%, firmware converts to appropriate value.
{"set":"amp", "to":20}	# command sends a percentage value 0-100%, firmware converts to appropriate value.

# Firmware -> User interface
{"t":<timestamp>, "set":"freq", "param":25}	#response that frequency has been set with frequency in Hertz returned in param.
{"t":<timestamp>, "set":"amp", "param":20}	#response that amplitude has been set with amplitude as percentage (or in absolute terms?) returned in param.
{"t":<timestamp>, "error":"timeout", "message":"Vibrations timed out, please start again if desired"}	# possible timeout error response
