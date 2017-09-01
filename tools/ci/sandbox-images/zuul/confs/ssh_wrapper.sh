#!/bin/bash
# shellcheck disable=SC2029
ssh -o StrictHostKeyChecking=no -i /var/lib/keys/id_rsa "$@"