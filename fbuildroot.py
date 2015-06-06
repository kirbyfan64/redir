from fbuild.builders.platform import guess_platform
from fbuild.builders.c import guess_static
from optparse import make_option
import fbuild

def pre_options(parser):
    group = parser.add_option_group('config options')
    group.add_options((
        make_option('--cc', help='Use the given C compiler'),
        make_option('--use-colors', help='Force the C compiler use colors',
                    action='store_true'),
        make_option('--config', help='Configuration', choices=['debug','release'],
                    default='debug'),
    ))

def configure(ctx):
    if not guess_platform(ctx) & {'posix'}:
        raise fbuild.Error('this only works on POSIX systems (try using MinGW)')
    return guess_static(ctx, flags=['-std=c89', '-pedantic', '-Wall', '-Werror']\
        +(['-fcolor-diagnostics'] if ctx.options.use_colors else []),
        macros=['_POSIX_C_SOURCE=200112L'],
        **({'debug': True} if ctx.options.config == 'debug'\
           else {'optimize': True}))

def build(ctx):
    c = configure(ctx)
    redir = c.build_exe('redir', ['redir.c'])
    redirf = c.build_exe('redirf', ['redir.c'], macros=['REDIRF'])
    for app in 'redir', 'redirf':
        bin = c.build_exe(app, ['redir.c'],\
                          macros=['REDIRF'] if app == 'redirf' else [])
        ctx.install(bin, 'bin')
        ctx.install(app + '.man', 'share', 'man/man1')
