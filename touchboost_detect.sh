#!/bin/sh

for i in /sys/devices/system/cpu/cpufreq/policy*; do
	if [ -d "${i}" ]; then
		echo "${i}/scaling_min_freq" `cat "${i}/scaling_min_freq"` `cat "${i}/scaling_max_freq"`
	fi
done

for i in /sys/devices/platform/*/devfreq/*; do
	if [ -d "${i}" ]; then
		echo "${i}/min_freq" `cat "${i}/min_freq"` `cat "${i}/max_freq"`
	fi
done