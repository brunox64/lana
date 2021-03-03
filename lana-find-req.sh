#!/bin/zsh

pesquisa=$1

./lana.bin -g1 "\\[scheduler-tag-sync-jobs_Worker-(\d+)\\]" -m "$pesquisa" | \
./lana.bin -g1 "request_id=([a-zA-Z0-9\-]+)" -m "$pesquisa"
