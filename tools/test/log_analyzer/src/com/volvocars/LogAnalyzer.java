package com.volvocars;

import com.android.ddmlib.testrunner.TestIdentifier;
import com.android.loganalysis.item.AnrItem;
import com.android.loganalysis.item.JavaCrashItem;
import com.android.loganalysis.item.LogcatItem;
import com.android.loganalysis.item.MiscLogcatItem;
import com.android.loganalysis.item.NativeCrashItem;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice;
import com.android.tradefed.log.LogUtil;
import com.android.tradefed.result.ITestInvocationListener;
import com.android.tradefed.result.InputStreamSource;
import com.android.tradefed.testtype.IRemoteTest;
import com.android.tradefed.testtype.IDeviceTest;
import com.android.tradefed.device.LogcatReceiver;
import com.android.tradefed.util.FileUtil;
import com.android.tradefed.util.Pair;
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
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


/**
 * This test will run logcat for a few seconds and then verify that no module is
 * producing too much log
 */
public class LogAnalyzer implements IRemoteTest, IDeviceTest {
    private ITestDevice mDevice;

    // LogcatParser requires you to setup match-patterns which it will place into "categories"
    // I call these magic categories. They are setup with addPattern and can be extracted with getMiscEvents.
    // It is not possible to filter by log level in any other way
    private final String MAGIC_CATEGORY_VERBOSE = "LogAnalyzerCategory_verbose";
    private final String MAGIC_CATEGORY_DEBUG = "LogAnalyzerCategory_debug";
    private final String MAGIC_CATEGORY_INFO = "LogAnalyzerCategory_info";
    private final String MAGIC_CATEGORY_WARNING = "LogAnalyzerCategory_warning";
    private final String MAGIC_CATEGORY_ERROR = "LogAnalyzerCategory_warning";
    private final String MAGIC_CATEGORY_FATAL = "LogAnalyzerCategory_fatal";
    private final String MAGIC_CATEGORY_CHATTY = "LogAnalyzerCategory_chatty";

    private final List<Pair<String,String>> verbosityToMagicCategoryMap = Arrays.asList(
            new Pair<String,String>("V", MAGIC_CATEGORY_VERBOSE),
            new Pair<String,String>("D", MAGIC_CATEGORY_DEBUG),
            new Pair<String,String>("I", MAGIC_CATEGORY_INFO),
            new Pair<String,String>("W", MAGIC_CATEGORY_WARNING),
            new Pair<String,String>("E", MAGIC_CATEGORY_ERROR),
            new Pair<String,String>("F", MAGIC_CATEGORY_FATAL)
    );

    private final float measurementTime_sec = 20;
    private final List<String> verbosityLevels = Arrays.asList(
            "F",
            "E",
            "W",
            "I",
            "D",
            "V"
    );

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

        try {
            LogcatItem logcat = getLogcat(measurementTime_sec);

            List<String> failures = new ArrayList<>();

            LogUtil.CLog.d("LOGCATITEM: %s", logcat.toString());
            LogUtil.CLog.d("LOGCATITEM.events: %d", logcat.getEvents().size());
            LogUtil.CLog.d("LOGCATITEM.anrs: %d", logcat.getAnrs().size());
            LogUtil.CLog.d("LOGCATITEM.nativecrashes: %d", logcat.getNativeCrashes().size());
            LogUtil.CLog.d("LOGCATITEM.javaCrashes: %d", logcat.getJavaCrashes().size());
            LogUtil.CLog.d("LOGCATITEM.stop: %s", logcat.getStopTime().toString());
            LogUtil.CLog.d("LOGCATITEM.start: %s", logcat.getStartTime().toString());

            failures.addAll(analyzeChatty(logcat));
            failures.addAll(analyzeCrashItems(logcat));
            failures.addAll(analyzePlainLogEntries(logcat));

            if (failures.size() > 0) {
                StringBuilder errMsg = new StringBuilder();
                errMsg.append("Application spamming the logs found\n");
                errMsg.append("See https://c1.confluence.cm.volvocars.biz/display/IHUA/Logging for logging guidelines\n");
                errMsg.append("---------------------------------------------------------------------------------------\n");
                errMsg.append("List of violations: \n");
                for (String s : failures) {
                    boolean firstLine = true;
                    for (String line : s.split("\\r?\\n")) {
                        if (firstLine) {
                            firstLine = false;
                            errMsg.append(" - ");
                        } else {
                            errMsg.append(".    ");
                        }
                        errMsg.append(line).append("\n");
                    }
                }
                listener.testFailed(testId, errMsg.toString());
            }

            listener.testEnded(testId, Collections.<String, String>emptyMap());
            listener.testRunEnded(0, Collections.<String, String>emptyMap());
        } catch (Exception e) {
            LogUtil.CLog.e(e);
            listener.testFailed(testId, String.format("Exception occured during test run: %s", e.getMessage()));
            listener.testEnded(testId, Collections.<String, String>emptyMap());
            listener.testRunEnded(0, Collections.<String, String>emptyMap());
        }
    }


    //NOTE: There is a similar tool in AOSP/development/tools/logblame/analyze_logs.py
    //      It does very similar analysis but it does not have any hard limits, just printing.
    private  List<String> analyzePlainLogEntries(LogcatItem logcat) throws DeviceNotAvailableException {
        List<String> failures = new ArrayList<>();
        Map<String, Integer> logCountByTag = new HashMap<>();
        Map<String, Map<String, Integer>> logCountByCategoryByTag = new HashMap<>();
        for (String level : verbosityLevels)
            logCountByCategoryByTag.put(level, new HashMap<>());

        Map<Integer, Integer> logCountByPid = new HashMap<>();
        Map<String, Map<Integer, Integer>> logCountByCategoryAndPid = new HashMap<>();
        for (String level : verbosityLevels)
            logCountByCategoryAndPid.put(level, new HashMap<>());


        for (Pair<String,String> verbosityLevel : verbosityToMagicCategoryMap) {
            final String verbosity = verbosityLevel.first;
            final String category = verbosityLevel.first;

            for (MiscLogcatItem e : logcat.getMiscEvents(category)) {
                if (e.getTag().trim().equals("")) {
                    String process = getProcessNameFromPid(e.getPid());
                    if ("zygote".equals(process) || "zygote64".equals(process))
                        continue;
                    String err = String.format(Locale.ROOT, "Log entry without log tag. PID: %d, cmd:%s, Msg:%s",
                            e.getPid(),
                            process,
                            e.getStack());
                    failures.add(err);
                } else {
                    if ("F".equals(verbosity) && "libc".equals(e.getTag()) && e.getStack() != null
                            && "FORTIFY: FD_SET: file descriptor -1 < 0".equals(e.getStack().trim()))
                    {
                        //TODO: Temporary exclusion
                        LogUtil.CLog.i("Ignoring fatal in libc: %s", e.getStack());
                        continue;
                    }

                    Increase(logCountByPid, e.getPid());
                    Increase(logCountByTag, e.getTag());
                    Increase(logCountByCategoryByTag.get(verbosity), e.getTag());
                    Increase(logCountByCategoryAndPid.get(verbosity), e.getPid());
                }
            }
        }

        Map<String, Float> maxLogsPerSecPerLevelPerTag = new HashMap<>();
        for (String level : verbosityLevels)
            maxLogsPerSecPerLevelPerTag.put(level, 0.0f);
        maxLogsPerSecPerLevelPerTag.put("F", 0.0f);
        maxLogsPerSecPerLevelPerTag.put("E", 5.0f);
        maxLogsPerSecPerLevelPerTag.put("W", 5.0f);
        maxLogsPerSecPerLevelPerTag.put("I", 10.0f);
        maxLogsPerSecPerLevelPerTag.put("D", 30.0f);
        maxLogsPerSecPerLevelPerTag.put("V", 50.0f);

        Map<String, Float> maxLogsPerSecPerLevelPerPid = new HashMap<>();
        for (String level : verbosityLevels)
            maxLogsPerSecPerLevelPerPid.put(level, 0.0f);
        maxLogsPerSecPerLevelPerPid.put("F", 0.0f);
        maxLogsPerSecPerLevelPerPid.put("E", 5.0f);
        maxLogsPerSecPerLevelPerPid.put("W", 5.0f);
        maxLogsPerSecPerLevelPerPid.put("I", 10.0f);
        maxLogsPerSecPerLevelPerPid.put("D", 30.0f);
        maxLogsPerSecPerLevelPerPid.put("V", 50.0f);


        //TODO: Temporary exclusions
        HashSet<String> tempExcludedTags = new HashSet<String>(Arrays.asList(
                "##PollingService##rvc##",
                "StreamHAL",
                "SLCANProto",
                "libc",
                "iplmd.service",
                "DPTF",
                "chatty",
                "/system/bin/hwservicemanager",  // https://flow.jira.cm.volvocars.biz/browse/PSS370-4236
                "IptablesRestoreController",
                "android.hardware.power@1.0::Power"

        ));
        //TODO: Temporary exclusions
        HashSet<String> tempExcludedProcesses = new HashSet<String>(Arrays.asList(
                "/vendor/bin/hw/android.hardware.audio@2.0-service",
                "/vendor/bin/hw/android.hardware.automotive.vehicle@2.0-service",
                "com.intel.rvc",
                "system_server",
                "/vendor/bin/hw/iplmd",
                "/system/vendor/bin/esif_ufd",
                "/system/bin/netd",
                "/system/bin/hwservicemanager"  // https://flow.jira.cm.volvocars.biz/browse/PSS370-4236
        ));


        for (String level : verbosityLevels) {
            Map<String, Integer> logsByTagForThisLevel = logCountByCategoryByTag.get(level);
            Map<Integer, Integer> logsByPidForThisLevel = logCountByCategoryAndPid.get(level);


            for (Map.Entry<String, Integer> tagAndCount : logsByTagForThisLevel.entrySet()) {
                final String tag = tagAndCount.getKey();
                final int nrOfEntries = tagAndCount.getValue();

                float entriesPerSec = nrOfEntries / measurementTime_sec;
                float max = maxLogsPerSecPerLevelPerTag.get(level);

                if (entriesPerSec > max) {
                    String err = String.format(Locale.ROOT, "Log tag='%s' is exceeding max logs per sec for level=%s. Max=%.2f, Measured=%.2f",
                            tag, level, max, entriesPerSec);
                    if (tempExcludedTags.contains(tag.trim())) {
                        LogUtil.CLog.d("Ignoring violation in log tag %s: %s", tag, err);
                        continue;
                    }

                    failures.add(err);
                }
            }

            for (Map.Entry<Integer, Integer> pidAndCount : logsByPidForThisLevel.entrySet()) {
                final Integer pid = pidAndCount.getKey();
                final int nrOfEntries = pidAndCount.getValue();

                String processName = getProcessNameFromPid(pid);
                float entriesPerSec = nrOfEntries / measurementTime_sec;
                float max = maxLogsPerSecPerLevelPerPid.get(level);

                if (entriesPerSec > max) {
                    String err = String.format(Locale.ROOT, "Process pid=%d cmd=%s is exceeding max logs per sec for level=%s. Max=%.2f, Measured=%.2f",
                            pid, processName, level, max, entriesPerSec);

                    if (tempExcludedProcesses.contains(processName)) {
                        LogUtil.CLog.i("Ignoring violation in process %s: %s", processName, err);
                        continue;
                    }

                    failures.add(err);
                }
            }
        }
        return failures;
    }


    private  List<String> analyzeCrashItems(LogcatItem logcat) {
        List<String> failures = new ArrayList<>();

        //ANR = Application Not Responding
        for (AnrItem anrItem : logcat.getAnrs()) {

            //TODO: Temporary exclusions
            if (anrItem.getReason() != null && anrItem.getReason().contains("com.android.server.telecom/.components.TelecomService"))
                continue;
            if (anrItem.getActivity() == null && anrItem.getReason() == null && anrItem.getTrace() == null && anrItem.getStack() != null && anrItem.getStack().trim().equals("ANR in system"))
                continue;

            String err = String.format(("ANR.CPU: %s \n" +
                            "ANR.Load: %s\n" +
                            "ANR.Activity: %s\n" +
                            "ANR.Reason: %s\n" +
                            "ANR.trace: %s\n" +
                            "ANR.stack: %s\n"),
                    anrItem.getCpuUsage(AnrItem.CpuUsageCategory.TOTAL),
                    anrItem.getLoad(AnrItem.LoadCategory.LOAD_5),
                    anrItem.getActivity(),
                    anrItem.getReason(),
                    anrItem.getTrace(),
                    anrItem.getStack());
            failures.add(err);
        }
        for (NativeCrashItem ci : logcat.getNativeCrashes()) {
            String err = String.format("NativeCrashItem: %s", ci.getStack());
            failures.add(err);
        }
        for (JavaCrashItem ci : logcat.getJavaCrashes()) {
            String err = String.format("JavaCrashItem: %s", ci.getStack());
            failures.add(err);
        }
        return failures;
    }


    private List<String> analyzeChatty(LogcatItem logcat) {
        //TODO: We could also disable chatty pruning before running this test with logcat -P ?


        List<String> failures = new ArrayList<>();
        //TODO: Temporary exclusions
        HashSet<String> tempExcludedChattyUids = new HashSet<String>(Arrays.asList(
                "audioserver"
        ));

        //TODO: Temporary exclusions
        HashSet<String> tempExcludedChattyProcesses = new HashSet<String>(Arrays.asList(
                "Thread-1",
                "system_server",
                "/system/bin/audioserver",
                "audio@2.0-servi",
                "AudioOut_D",
                "VolumeDialogCon",
                "system-server-i",
                "sensors@1.0-ser",
                "SensorService",
                "InputReader",
                "hwservicemanage"
        ));

        /* Chatty is a component of the logging system detecting log spam in run time
           When spam is detected, multiple log entries from the spammy process will be replaced with a
           single log entry only containing the number of logs.
           Example of chatty output:
                11-11 11:21:58.611  2367  2374 I chatty  : uid=0(root) vehicle-signals expire 153 lines
                11-11 11:21:58.687  2957  3065 I chatty  : uid=1000(system) Thread-1 identical 5 lines
                11-11 11:21:58.947  2306  2318 I chatty  : uid=0(root) /vendor/bin/hw/ipcbd expire 6 lines
                11-11 11:21:58.948  2308  2310 I chatty  : uid=0(root) /vendor/bin/hw/iplmd expire 7 lines

            This regexp will match: uid, process and "expire" vs "identical"
            LogCatReceiver will parse the time/pid/thread/etc and only give us the message with getStacK()
        */
        Pattern chattyPattern = Pattern.compile(
                "uid=(\\d+)(\\(\\S*\\))?\\s+" +
                "(.*?)\\s+" +
                "(expire|identical)\\s+" +
                "(\\d+)\\s+lines?"
        );
        for (MiscLogcatItem e : logcat.getMiscEvents(MAGIC_CATEGORY_CHATTY)) {

            Matcher match = chattyPattern.matcher(e.getStack());
            String process = "unknown";
            String uidMatch = "";
            String type = "unknown";
            int nrOf = 0;

            if (match.matches())
            {
                uidMatch = match.group(2);
                process = match.group(3);
                type = match.group(4);
                nrOf = Integer.parseInt(match.group(5));
            }

            if ("identical".equals(type)) {
                // give some slack :)
                if (nrOf < 3) {
                    continue;
                }
            }

            if (tempExcludedChattyUids.contains(uidMatch)) {
                LogUtil.CLog.i("Ignoring chatty violation from uid=%s", uidMatch);
                continue;
            }

            if (tempExcludedChattyProcesses.contains(process)) {
                LogUtil.CLog.i("Ignoring chatty violation from %s", process);
                continue;
            }
            if (process.startsWith("Binder") || process.startsWith("HwBinder"))
            {
                LogUtil.CLog.i("Ignoring chatty violation from %s", process);
                continue;
            }

            String err = String.format("Chatty classified logspam: %s", e.getStack());
            failures.add(err);
        }
        return failures;
    }

    private <TKey> void Increase(Map<TKey, Integer> map, TKey key) {
        int count = map.containsKey(key) ? map.get(key) : 0;
        map.put(key, count + 1);
    }


    private LogcatItem parseLogCat(InputStreamSource logcatStream) throws IOException {
        LogcatParser parser = new LogcatParser();

        for (Pair<String, String> p: verbosityToMagicCategoryMap)
            parser.addPattern(null, p.first, null, p.second);
        parser.addPattern(null, null, "chatty", MAGIC_CATEGORY_CHATTY);
        try (InputStream stream = logcatStream.createInputStream()) {
            try (BufferedReader br = new BufferedReader(new InputStreamReader(stream))) {
                return  parser.parse(br);
            }
        }
    }

    private LogcatItem getLogcat(final float measurementTime_sec) throws IOException {
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
            File f = getDevice().pullFile(String.format(Locale.ROOT, "/proc/%d/cmdline", pid));
            if (f == null) {
                LogUtil.CLog.w("Failed to read cmdline for pid: %d. File does not exist.", pid);
                return "unknown";
            } else {
                return FileUtil.readStringFromFile(f).split("\0")[0];
            }
        } catch (IOException e) {
            LogUtil.CLog.w("Failed to read cmdline for pid: %d. %s", pid, e.toString());
            return "unknown";
        }
    }
}
