#include <Python.h>
#include <stdint.h>

// hawkey
#include "src/repo.h"

// pyhawkey
#include "hawkey-pysys.h"
#include "repo-py.h"

typedef struct {
    PyObject_HEAD
    HyRepo repo;
} _RepoObject;

HyRepo repoFromPyObject(PyObject *o)
{
    if (!repoObject_Check(o)) {
	PyErr_SetString(PyExc_TypeError, "Expected a Repo object.");
	return NULL;
    }
    return ((_RepoObject *)o)->repo;
}

PyObject *repoToPyObject(HyRepo repo)
{
    _RepoObject *self = (_RepoObject *)repo_Type.tp_alloc(&repo_Type, 0);
    if (self)
	self->repo = repo;
    return (PyObject *)self;
}

/* functions on the type */

static PyObject *
repo_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    _RepoObject *self = (_RepoObject *)type->tp_alloc(type, 0);
    if (self) {
	self->repo = hy_repo_create();
	if (self->repo == NULL) {
	    Py_DECREF(self);
	    return NULL;
	}
    }
    return (PyObject *) self;
}

static void
repo_dealloc(_RepoObject *self)
{
    hy_repo_free(self->repo);
    Py_TYPE(self)->tp_free(self);
}

/* getsetters */

static PyObject *
get_str(_RepoObject *self, void *closure)
{
    int str_key = (intptr_t)closure;
    const char *str;
    PyObject *ret;

    str = hy_repo_get_string(self->repo, str_key);
    if (str == NULL) {
	ret = PyString_FromString("");
    } else {
	ret = PyString_FromString(str);
    }
    return ret; // NULL if PyString_FromString failed
}

static int
set_str(_RepoObject *self, PyObject *value, void *closure)
{
    intptr_t str_key = (intptr_t)closure;
    const char *str_value;

    str_value = PyString_AsString(value);
    if (str_value == NULL)
	return -1;
    hy_repo_set_string(self->repo, str_key, str_value);

    return 0;
}

static PyGetSetDef repo_getsetters[] = {
    {"name", (getter)get_str, (setter)set_str, NULL,
     (void *)HY_REPO_NAME},
    {"repomd_fn", (getter)get_str, (setter)set_str, NULL,
     (void *)HY_REPO_MD_FN},
    {"primary_fn", (getter)get_str, (setter)set_str, NULL,
     (void *)HY_REPO_PRIMARY_FN},
    {"filelists_fn", (getter)get_str, (setter)set_str, NULL,
     (void *)HY_REPO_FILELISTS_FN},
    {"presto_fn", (getter)get_str, (setter)set_str, NULL,
     (void *)HY_REPO_PRESTO_FN},
    {NULL}			/* sentinel */
};

PyTypeObject repo_Type = {
    PyObject_HEAD_INIT(NULL)
    0,				/*ob_size*/
    "hawkey.Repo",		/*tp_name*/
    sizeof(_RepoObject),	/*tp_basicsize*/
    0,				/*tp_itemsize*/
    (destructor) repo_dealloc,  /*tp_dealloc*/
    0,				/*tp_print*/
    0,				/*tp_getattr*/
    0,				/*tp_setattr*/
    0,				/*tp_compare*/
    0,				/*tp_repr*/
    0,				/*tp_as_number*/
    0,				/*tp_as_sequence*/
    0,				/*tp_as_mapping*/
    0,				/*tp_hash */
    0,				/*tp_call*/
    0,				/*tp_str*/
    0,				/*tp_getattro*/
    0,				/*tp_setattro*/
    0,				/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,		/*tp_flags*/
    "Repo object",		/* tp_doc */
    0,				/* tp_traverse */
    0,				/* tp_clear */
    0,				/* tp_richcompare */
    0,				/* tp_weaklistoffset */
    PyObject_SelfIter,		/* tp_iter */
    0,                         	/* tp_iternext */
    0,				/* tp_methods */
    0,				/* tp_members */
    repo_getsetters,		/* tp_getset */
    0,				/* tp_base */
    0,				/* tp_dict */
    0,				/* tp_descr_get */
    0,				/* tp_descr_set */
    0,				/* tp_dictoffset */
    0,				/* tp_init */
    0,				/* tp_alloc */
    repo_new,			/* tp_new */
    0,				/* tp_free */
    0,				/* tp_is_gc */
};
