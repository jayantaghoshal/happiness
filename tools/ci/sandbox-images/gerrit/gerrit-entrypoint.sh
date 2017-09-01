#!/usr/bin/env sh
set -e

set_gerrit_config() {
  git config -f "${GERRIT_SITE}/etc/gerrit.config" "$@"
}

# If you're mounting ${GERRIT_SITE} to your host, you this will default to root.
# This obviously ensures the permissions are set correctly for when gerrit starts.
chown -R "${GERRIT_USER}" "${GERRIT_HOME}/gerrit.war"
chown -R "${GERRIT_USER}" "${GERRIT_SITE}"
echo "Before logged in as gerrit2"
# shellcheck disable=SC2117
su "${GERRIT_USER}"
echo "Logged in as gerrit2"

if [ "$1" = "start" ]; then

  # Initialize Gerrit if ${GERRIT_SITE}/git is empty.
  if [ -z "$(ls -A "$GERRIT_SITE/git")" ]; then
    echo "First time initialize gerrit..."
    java "${JAVA_OPTIONS}" "${JAVA_MEM_OPTIONS}" -jar "${GERRIT_WAR}" init --no-auto-start --batch --install-all-plugins -d "${GERRIT_SITE}"    
    set_gerrit_config auth.type "DEVELOPMENT_BECOME_ANY_ACCOUNT"
    set_gerrit_config change.submitWholeTopic "true"
    set_gerrit_config gerrit.canonicalWebUrl "http://dock_gerrit:8080/"    


    # Init zuul user
    #java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar ${GERRIT_WAR} gsql -d ${GERRIT_SITE} -c "insert into ACCOUNTS values (NULL, 'zuul', NULL, NULL, 'N', NULL, NULL, NULL, NULL, 25, 'N', 'N', 'Y', 'N', NULL, 'Y', 'N', 'zuul@localhost', '2015-05-28 11:00:30.001', 2)"
    #java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar ${GERRIT_WAR} gsql -d ${GERRIT_SITE} -c "insert into ACCOUNT_GROUP_MEMBERS values (2, 4)"
    #java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar ${GERRIT_WAR} gsql -d ${GERRIT_SITE} -c "insert into ACCOUNT_EXTERNAL_IDS values (2, 'zuul@localhost', NULL, 'username:zuul')"
    #java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar ${GERRIT_WAR} gsql -d ${GERRIT_SITE} -c "insert into ACCOUNT_EXTERNAL_IDS values (2, 'zuul@localhost', NULL, 'mailto:zuul@localhost')"
    #pubkey=$(cat /zuul_id_rsa.pub)
    #java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar ${GERRIT_WAR} gsql -d ${GERRIT_SITE} -c "insert into account_ssh_keys values ('$pubkey', 'Y', 2, 1)"
    #java ${JAVA_OPTIONS} ${JAVA_MEM_OPTIONS} -jar ${GERRIT_WAR} gsql -d ${GERRIT_SITE} -c "insert into account_ssh_keys values ('$pubkey', 'Y', 1, 1)"
  fi


  cat "${GERRIT_SITE}/etc/gerrit.config"

  "$GERRIT_SITE"/bin/gerrit.sh "${GERRIT_START_ACTION:-daemon}"





fi
exec "$@"