#include <assert.h>

// libsolv
#include "solv/util.h"

// hawkey
#include "repo_internal.h"


// internal functions

HyRepo
hy_repo_link(HyRepo repo)
{
    repo->nrefs++;
    return repo;
}

int
hy_repo_transition(HyRepo repo, enum _hy_repo_state new_state)
{
    int trans = repo->state << 8 | new_state;
    switch (trans) {
    case _HY_NEW << 8		    | _HY_LOADED_FETCH:
    case _HY_NEW << 8		    | _HY_LOADED_CACHE:
    case _HY_LOADED_FETCH << 8	    | _HY_WRITTEN:
    case _HY_LOADED_FETCH << 8	    | _HY_FL_LOADED_FETCH:
    case _HY_LOADED_FETCH << 8	    | _HY_FL_LOADED_CACHE:
    case _HY_LOADED_CACHE << 8	    | _HY_FL_LOADED_FETCH:
    case _HY_LOADED_CACHE << 8	    | _HY_FL_LOADED_CACHE:
    case _HY_WRITTEN << 8	    | _HY_FL_LOADED_FETCH:
    case _HY_WRITTEN << 8	    | _HY_FL_LOADED_CACHE:
    case _HY_FL_LOADED_FETCH << 8   | _HY_FL_WRITTEN:
    case _HY_LOADED_FETCH << 8	    | _HY_PST_LOADED_FETCH:
    case _HY_LOADED_FETCH << 8	    | _HY_PST_LOADED_CACHE:
    case _HY_LOADED_CACHE << 8	    | _HY_PST_LOADED_FETCH:
    case _HY_LOADED_CACHE << 8	    | _HY_PST_LOADED_CACHE:
    case _HY_WRITTEN << 8	    | _HY_PST_LOADED_FETCH:
    case _HY_WRITTEN << 8	    | _HY_PST_LOADED_CACHE:
    case _HY_FL_LOADED_FETCH << 8   | _HY_PST_LOADED_FETCH:
    case _HY_FL_LOADED_FETCH << 8   | _HY_PST_LOADED_CACHE:
    case _HY_FL_LOADED_CACHE << 8   | _HY_PST_LOADED_FETCH:
    case _HY_FL_LOADED_CACHE << 8   | _HY_PST_LOADED_CACHE:
    case _HY_FL_WRITTEN << 8	    | _HY_PST_LOADED_FETCH:
    case _HY_FL_WRITTEN << 8	    | _HY_PST_LOADED_CACHE:
    case _HY_PST_LOADED_FETCH << 8  | _HY_PST_WRITTEN:
	repo->state = new_state;
	return 0;
    default:
	return 1;
    }
}

Id
repo_get_repodata(HyRepo repo, enum _hy_repo_repodata which)
{
    switch (which) {
    case _HY_REPODATA_FILENAMES:
	return repo->filenames_repodata;
    case _HY_REPODATA_PRESTO:
	return repo->presto_repodata;
    default:
	assert(0);
	return 0;
    }
}

void
repo_set_repodata(HyRepo repo, enum _hy_repo_repodata which, Id repodata)
{
    switch (which) {
    case _HY_REPODATA_FILENAMES:
	repo->filenames_repodata = repodata;
	return;
    case _HY_REPODATA_PRESTO:
	repo->presto_repodata = repodata;
	return;
    default:
	assert(0);
	return;
    }
}

// public functions

HyRepo
hy_repo_create(void)
{
    HyRepo repo = solv_calloc(1, sizeof(*repo));
    repo->nrefs = 1;
    repo->state = _HY_NEW;
    return repo;
}

void
hy_repo_set_string(HyRepo repo, enum _hy_repo_param_e which, const char *str_val)
{
    switch (which) {
    case HY_REPO_NAME:
	repo->name = solv_strdup(str_val);
	break;
    case HY_REPO_MD_FN:
	repo->repomd_fn = solv_strdup(str_val);
	break;
    case HY_REPO_PRIMARY_FN:
	repo->primary_fn = solv_strdup(str_val);
	break;
    case HY_REPO_FILELISTS_FN:
	repo->filelists_fn = solv_strdup(str_val);
	break;
    case HY_REPO_PRESTO_FN:
	repo->presto_fn = solv_strdup(str_val);
	break;
    default:
	assert(0);
    }
}

const char *
hy_repo_get_string(HyRepo repo, enum _hy_repo_param_e which)
{
    switch(which) {
    case HY_REPO_NAME:
	return repo->name;
    case HY_REPO_MD_FN:
	return repo->repomd_fn;
    case HY_REPO_PRIMARY_FN:
	return repo->primary_fn;
    case HY_REPO_FILELISTS_FN:
	return repo->filelists_fn;
    case HY_REPO_PRESTO_FN:
	return repo->presto_fn;
    default:
	assert(0);
    }
    return NULL;
}

void
hy_repo_free(HyRepo repo)
{
    if (--repo->nrefs > 0)
	return;

    solv_free(repo->name);
    solv_free(repo->repomd_fn);
    solv_free(repo->primary_fn);
    solv_free(repo->filelists_fn);
    solv_free(repo->presto_fn);
    solv_free(repo);
}
