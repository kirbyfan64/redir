redir
=====

``redir`` removes the UNIX pipeline restriction that `a pipeline cannot read from and write to the same file <http://stackoverflow.com/questions/1895981/why-reading-and-writing-the-same-file-through-i-o-redirection-results-in-an-empt>`_.

``redir`` comes with two binaries: ``redir`` and ``redirf``. The former uses an internal buffer, while the latter uses a temporary file.

Building
********

You need `pconfigure <https://github.com/palmer-dabbelt/pconfigure>`_ to build ``redir``. Just run::
   
   $ pconfigure
   $ make
   $ sudo make install

To build with debug symbols, use ``pconfigure --config debug``.

Usage and examples
******************

See ``redir.1``.

Todo
****

- Add more examples
