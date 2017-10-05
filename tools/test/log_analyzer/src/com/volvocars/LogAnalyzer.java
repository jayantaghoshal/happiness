package com.volvocars;

import com.android.ddmlib.testrunner.TestIdentifier;
import com.android.loganalysis.item.LogcatItem;
import com.android.loganalysis.item.MiscLogcatItem;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice;
import com.android.tradefed.log.LogUtil;
import com.android.tradefed.result.ITestInvocationListener;
import com.android.tradefed.result.InputStreamSource;
import com.android.tradefed.testtype.IRemoteTest;
import com.android.tradefed.testtype.IDeviceTest;
import com.android.tradefed.device.LogcatReceiver;
import com.android.tradefed.util.FileUtil;
import com.android.tradefed.util.RunUtil;
import com.android.loganalysis.parser.LogcatParser;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * This test will run logcat for a few seconds and then verify that no module is
 * producing too much log
 */
public class LogAnalyzer implements IRemoteTest, IDeviceTest {
    private ITestDevice mDevice;

    @Override
    public void setDevice(ITestDevice device) {
        mDevice = device;
    }

    @Override
    public ITestDevice getDevice() {
        return mDevice;
    }


    @Override
    public void run(ITestInvocationListener listener) throws DeviceNotAvailableException {
        TestIdentifier testId = new TestIdentifier(getClass().getCanonicalName(), "LogSpamDetector");
        listener.testRunStarted("LogSpamDetector", 1);
        listener.testStarted(testId);
        final float measurementTime_sec = 10;
        final List<String> levels = Arrays.asList("F", "E", "W", "I", "D", "V");

        Map<String, Integer> allByTag = new HashMap<>();
        Map<String, Map<String, Integer>> categoryByTag = new HashMap<>();
        for (String l : levels)
            categoryByTag.put(l, new HashMap<>());

        Map<Integer, Integer> allByPid = new HashMap<>();
        Map<String, Map<Integer, Integer>> categoryByPid = new HashMap<>();
        for (String l : levels)
            categoryByPid.put(l, new HashMap<>());

        try {
            List<MiscLogcatItem> logcatItems = getLogcat(measurementTime_sec);

            List<String> failures = new ArrayList<>();

            for (MiscLogcatItem e : logcatItems) {
                if (e.getTag().equals("")) {
                    String err = String.format("Log entry without log tag. PID: %d", e.getPid());
                    failures.add(err);
                } else if (e.getTag().equals("chatty")) {
                    //TODO: We could also disable pruning before running this test with logcat -P ?
                    String err = String.format("Chatty classified logspam: %s", e.getStack());
                    failures.add(err);
                } else {
                    if (e.getTag().length() > 40) {
                        String err = String.format("Too long log tag: %s", e.getTag());
                        listener.testFailed(testId, err);
                    }

                    Increase(allByPid, e.getPid());
                    Increase(allByTag, e.getTag());
                    Increase(categoryByTag.get(e.getCategory()), e.getTag());
                    Increase(categoryByPid.get(e.getCategory()), e.getPid());
                }
            }

            Map<String, Float> maxLogsPerSecPerLevelGlobally = new HashMap<>();
            maxLogsPerSecPerLevelGlobally.put("F", 0.0f);
            maxLogsPerSecPerLevelGlobally.put("E", 5.0f);
            maxLogsPerSecPerLevelGlobally.put("W", 5.0f);
            maxLogsPerSecPerLevelGlobally.put("I", 10.0f);
            maxLogsPerSecPerLevelGlobally.put("D", 30.0f);
            maxLogsPerSecPerLevelGlobally.put("V", 50.0f);

            Map<String, Float> maxLogsPerSecPerLevelPerTag = new HashMap<>();
            maxLogsPerSecPerLevelPerTag.put("F", 0.0f);
            maxLogsPerSecPerLevelPerTag.put("E", 5.0f);
            maxLogsPerSecPerLevelPerTag.put("W", 5.0f);
            maxLogsPerSecPerLevelPerTag.put("I", 10.0f);
            maxLogsPerSecPerLevelPerTag.put("D", 30.0f);
            maxLogsPerSecPerLevelPerTag.put("V", 50.0f);

            Map<String, Float> maxLogsPerSecPerLevelPerPid = new HashMap<>();
            maxLogsPerSecPerLevelPerPid.put("F", 0.0f);
            maxLogsPerSecPerLevelPerPid.put("E", 0.5f);
            maxLogsPerSecPerLevelPerPid.put("W", 1.0f);
            maxLogsPerSecPerLevelPerPid.put("I", 10.0f);
            maxLogsPerSecPerLevelPerPid.put("D", 30.0f);
            maxLogsPerSecPerLevelPerPid.put("V", 50.0f);


            for (String l : levels) {
                Map<String, Integer> logsByTagForThisLevel = categoryByTag.get(l);
                for (Map.Entry<String, Integer> v : logsByTagForThisLevel.entrySet()) {
                    String tag = v.getKey();
                    int nrOfEntries = v.getValue();
                    float entriesPerSec = nrOfEntries / measurementTime_sec;
                    float max = maxLogsPerSecPerLevelPerTag.get(l);

                    if (entriesPerSec > max) {
                        String err = String.format("Log tag='%s' is exceeding max logs per sec for level=%s. Max=%.2f, Measured=%.2f",
                                tag, l, max, entriesPerSec);
                        failures.add(err);
                    }
                }

                Map<Integer, Integer> logsByPidForThisLevel = categoryByPid.get(l);
                for (Map.Entry<Integer, Integer> v : logsByPidForThisLevel.entrySet()) {
                    Integer pid = v.getKey();
                    int nrOfEntries = v.getValue();
                    float entriesPerSec = nrOfEntries / measurementTime_sec;
                    float max = maxLogsPerSecPerLevelPerPid.get(l);
                    if (entriesPerSec > max) {
                        String err = String.format("Process %s is exceeding max logs per sec for level=%s. Max=%.2f, Measured=%.2f",
                                getProcessNameFromPid(pid), l, max, entriesPerSec);
                        failures.add(err);
                    }
                }
            }


            if (failures.size() > 0) {
                StringBuilder errMsg = new StringBuilder();
                errMsg.append("Application spamming the logs found\n");
                errMsg.append("See https://c1.confluence.cm.volvocars.biz/display/IHUA/Logging for logging guidelines\n");
                errMsg.append("---------------------------------------------------------------------------------------\n");
                errMsg.append("List of violations: \n");
                for (String s : failures)
                    errMsg.append(" - ").append(s).append("\n");
                listener.testFailed(testId, errMsg.toString());
            }

            listener.testEnded(testId, Collections.emptyMap());
            listener.testRunEnded(0, Collections.emptyMap());
        } catch (Exception e) {
            LogUtil.CLog.e(e);
            listener.testFailed(testId, String.format("Exception occured during test run: %s", e.getMessage()));
            listener.testEnded(testId, Collections.emptyMap());
            listener.testRunEnded(0, Collections.emptyMap());
        }
    }

    private <TKey> void Increase(Map<TKey, Integer> map, TKey key) {
        int count = map.containsKey(key) ? map.get(key) : 0;
        map.put(key, count + 1);
    }


    private List<MiscLogcatItem> parseLogCat(InputStreamSource logcatStream) throws IOException {
        LogcatParser parser = new LogcatParser();
        try (InputStream stream = logcatStream.createInputStream()) {
            try (BufferedReader br = new BufferedReader(new InputStreamReader(stream))) {
                LogcatItem item = parser.parse(br);
                return item.getEvents();
            }
        }
    }

    private List<MiscLogcatItem> getLogcat(final float measurementTime_sec) throws IOException {
        final long avgLogEntrySize_bytes = 113;
        final long maxFileSize = (long) (avgLogEntrySize_bytes * 1000 * measurementTime_sec);
        final int logStartDelay = 0;

        LogcatReceiver rec = new LogcatReceiver(mDevice, "logcat", maxFileSize, logStartDelay);
        rec.start();
        RunUtil.getDefault().sleep((long) (measurementTime_sec * 1000));
        InputStreamSource logcatStream = rec.getLogcatData();
        rec.stop();
        try {
            return parseLogCat(logcatStream);
        } finally {
            logcatStream.cancel();
        }
    }


    private String getProcessNameFromPid(final int pid) throws DeviceNotAvailableException {
        try {
            File f = getDevice().pullFile(String.format("/proc/%d/cmdline", pid));
            if (f == null) {
                LogUtil.CLog.e("Failed to read cmdline for pid: %d. File does not exist.", pid);
                return String.format("pid=%d", pid);
            } else {
                String cmd = FileUtil.readStringFromFile(f);
                return String.format("pid=%d (cmd=%s)", pid, cmd);
            }
        } catch (IOException e) {
            LogUtil.CLog.e("Failed to read cmdline for pid: %d. %s", pid, e.toString());
            return String.format("pid=%d", pid);
        }
    }
}