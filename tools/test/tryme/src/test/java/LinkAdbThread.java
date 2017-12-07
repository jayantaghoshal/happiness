/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.io.OutputStreamWriter;

public class LinkAdbThread implements Runnable {
    private Boolean connected = false;

    public void run() {
        try {
            ProcessBuilder adbShell = new ProcessBuilder(
                    "adb", "shell");

            File logFileAdb = new File("out/logFileAdb.txt");
            adbShell.redirectOutput(logFileAdb);

            Process procAdbShell = adbShell.start();
            // If above failed, we should drop out with IOException, meaning we are now connected
            connected = true;

            BufferedWriter commandInput = new BufferedWriter(new OutputStreamWriter(procAdbShell.getOutputStream()));

            commandInput.write("uptime\n");
            commandInput.flush();

            // Do some stuff for a while
            int loopCount = 1;
            while (loopCount < 4) {
                System.out.println("LinkAdbThread: Running stuff!");
                Thread.sleep(1000);
                loopCount++;
            }

            procAdbShell.destroy();
        } catch (IOException eIo) {
            System.err.println("LinkAdbThread: Connection Failed!");
            System.exit(1);
        } catch (InterruptedException eInt) {
            System.err.println("LinkAdbThread: Thread Interrupted!");
            System.exit(1);
        }
    }

    public Boolean getConnected() {
        return this.connected;
    }
}
