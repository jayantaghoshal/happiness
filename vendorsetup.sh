function setup_volvocars_paths()
{
    local T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP."
        return
    fi
    local tools_path=$T/vendor/volvocars/tools

    export PATH=$tools_path/bin:$PATH
    # TODO: export PYTHONPATH=$tools_path/python/site-packages:$PYTHONPATH
    #       (or wherever we decide to move Python modules later on)
}

setup_volvocars_paths
