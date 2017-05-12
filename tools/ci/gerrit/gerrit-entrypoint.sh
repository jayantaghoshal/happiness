#!/usr/bin/env sh
set -e

set_gerrit_config() {
  git config -f "${GERRIT_SITE}/etc/gerrit.config" "$@"
}

# If you're mounting ${GERRIT_SITE} to your host, you this will default to root.
# This obviously ensures the permissions are set correctly for when gerrit starts.
chown -R ${GERRIT_USER} "${GERRIT_HOME}/gerrit.war"
chown -R ${GERRIT_USER} "${GERRIT_SITE}"
echo "Before logged in as gerrit2"
su gerrit2
echo "Logged in as gerrit2"

if [ "$1" = "start" ]; then
  # Initialize Gerrit if ${GERRIT_SITE}/git is empty.
  if [ -z "$(ls -A "$GERRIT_SITE/git")" ]; then
    echo "First time initialize gerrit..."
    java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar "${GERRIT_WAR}" init --no-auto-start --batch --install-all-plugins -d "${GERRIT_SITE}"    
    set_gerrit_config auth.type "DEVELOPMENT_BECOME_ANY_ACCOUNT"    
  fi

  cat "${GERRIT_SITE}/etc/gerrit.config"

  $GERRIT_SITE/bin/gerrit.sh ${GERRIT_START_ACTION:-daemon}

fi
exec "$@"