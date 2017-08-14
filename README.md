# Plock
simple way to prevent process to run while another process is running and sharing the same lock file under windows.

# Scenario
Execute process automatically while not supervising is ending can cause problems next time we will execute the same process again, the previous process maybe not finish his job while new one execute, to prevent it we need a way to wait until the first one will finish, we can do it using a share file between the processes, when plock need to execute file, it's try to lock the file, only if it's success it will execute the file, if not it will use the mode flag.

# Arguments
File Lock - the file of the lock.<br />
Execute File - the file that should execute under the lock.<br />
Arguments - the arguments of the executed file.<br />
Mode:<br />
      s - single try to lock the file<br />
      r - try to lock file recrusive every 1 second
      



