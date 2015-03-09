redir
=====

`redir` removes the UNIX pipeline restriction that `a pipeline cannot read from and write to the same file <http://stackoverflow.com/questions/1895981/why-reading-and-writing-the-same-file-through-i-o-redirection-results-in-an-empt>`_.

`redir` comes with two binaries: `redir` and `redirf`. The former uses an internal buffer, while the latter uses a temporary file.

Building
********

You need `pconfigure <https://github.com/palmer-dabbelt/pconfigure>`_ to build `redir`. Just run::
   
   $ pconfigure
   $ make
   $ sudo make install

Usage
*****

::
   
   redir [-s <max>] [-a] <file>
   redirf [-s <max>] [-a] [-t <tmp>] <file>

`-s`
----

Give a maximum buffer size in bytes. If the buffer size exceeds this amount, the program aborts.

`-a`
----

Append to the target file instead of overwriting it.

`-t <tmp>` (``redirf`` only)
------------------------

Use `<tmp>` as the temporary file. Normally, `redirf` uses ``/tmp/redir.$$``, where `$$` is the current process id.

Examples
********

Using grep to filter a files contents::
   
   $ cat myfile | grep 'pattern_here' | redir myfile

Todo
****

- Add a man page (need to add a `man language to pconfigure first <https://github.com/palmer-dabbelt/pconfigure/issues/5>`_)
- Add more examples
