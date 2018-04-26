------Config File Used-----

Start Simulator Configuration File
Version/Phase: 1.5
File Path: (Insert mdf)
CPU Scheduling Code: (Insert Strategy)
Quantum Time (cycles): 5
Memory Available (KB): 667
Processor Cycle Time (msec): 10
I/O Cycle Time (msec): 20
Log To: File
Log File Path: (Insert Log file)
End Simulator Configuration File.

------End Config------

  Currently all mdf files are the same, this is by design. While
Dr. Leverington asked us to make each file different, doing so makes illustrating
the correct running of the sim for different scheduling algorithms difficult to
follow. As such, each is the same to ensure that every run of the sim using a
different scheduling algorithm should result in a different output. Using a
consistent set of metadata operations ensures this, and allows for each
algorithm to be more easily recognizable by its output.
  This is something I have confirmed with Dr. Leverington to meet the standards
he has given us.

  Currently all tests work as intended, at least to the point that I have
tracked by following the running of each algorithm. I have also implemented
memory management into each algorithm and that should be demonstrated by the
tests. Current my system works on the linux server, so if you would like to copy
over to there, compile, and run it go for it. This should allow you to test
smaller files as needed.

It's been fun, I guess.

X X
---
