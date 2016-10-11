#!/bin/bash

TARGET=homework.tar

rm -f ${TARGET}
tar cvf ${TARGET} syn_process_?.c syn_thread_?.c report.pdf 2> /dev/null
