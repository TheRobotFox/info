let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/projects/info
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +1 info.h
badd +11 ANSI.c
badd +1 ANSI.h
badd +5 stream_internal.h
badd +1 printf.h
badd +6 printf.c
badd +1 buffer.h
badd +1 stream.c
badd +39 info.c
badd +1 format.h
badd +4 info_internal.h
badd +220 format.c
badd +1 buffer.c
badd +1 stream.h
badd +1 format_internal.h
badd +1 config.h
badd +2 ANSI_internal.h
badd +39 List/List.c
badd +1 List/List.h
badd +13 format_funcs.h
badd +61 format_funcs.c
badd +1 List
badd +1 main.c
argglobal
%argdel
set stal=2
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit info.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 38 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 89 + 104) / 209)
exe '2resize ' . ((&lines * 22 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 89 + 104) / 209)
exe 'vert 3resize ' . ((&columns * 119 + 104) / 209)
argglobal
balt format.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 5 - ((4 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 5
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("info_internal.h", ":p")) | buffer info_internal.h | else | edit info_internal.h | endif
if &buftype ==# 'terminal'
  silent file info_internal.h
endif
balt info.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("info.c", ":p")) | buffer info.c | else | edit info.c | endif
if &buftype ==# 'terminal'
  silent file info.c
endif
balt List/List.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 78 - ((41 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 78
normal! 025|
wincmd w
exe '1resize ' . ((&lines * 38 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 89 + 104) / 209)
exe '2resize ' . ((&lines * 22 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 89 + 104) / 209)
exe 'vert 3resize ' . ((&columns * 119 + 104) / 209)
tabnext
edit ANSI.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 38 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 80 + 104) / 209)
exe '2resize ' . ((&lines * 22 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 80 + 104) / 209)
exe 'vert 3resize ' . ((&columns * 128 + 104) / 209)
argglobal
balt ANSI_internal.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 012|
wincmd w
argglobal
if bufexists(fnamemodify("ANSI_internal.h", ":p")) | buffer ANSI_internal.h | else | edit ANSI_internal.h | endif
if &buftype ==# 'terminal'
  silent file ANSI_internal.h
endif
balt ANSI.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 5 - ((4 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 5
normal! 046|
wincmd w
argglobal
if bufexists(fnamemodify("ANSI.c", ":p")) | buffer ANSI.c | else | edit ANSI.c | endif
if &buftype ==# 'terminal'
  silent file ANSI.c
endif
balt info.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 18 - ((17 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 18
normal! 043|
wincmd w
exe '1resize ' . ((&lines * 38 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 80 + 104) / 209)
exe '2resize ' . ((&lines * 22 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 80 + 104) / 209)
exe 'vert 3resize ' . ((&columns * 128 + 104) / 209)
tabnext
edit buffer.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 60 + 104) / 209)
exe 'vert 2resize ' . ((&columns * 148 + 104) / 209)
argglobal
balt printf.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 14 - ((13 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 14
normal! 012|
wincmd w
argglobal
if bufexists(fnamemodify("buffer.c", ":p")) | buffer buffer.c | else | edit buffer.c | endif
if &buftype ==# 'terminal'
  silent file buffer.c
endif
balt buffer.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 82 - ((46 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 82
normal! 015|
wincmd w
exe 'vert 1resize ' . ((&columns * 60 + 104) / 209)
exe 'vert 2resize ' . ((&columns * 148 + 104) / 209)
tabnext
edit stream.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 30 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 78 + 104) / 209)
exe '2resize ' . ((&lines * 30 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 78 + 104) / 209)
exe 'vert 3resize ' . ((&columns * 130 + 104) / 209)
argglobal
balt stream_internal.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 15) / 30)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 012|
wincmd w
argglobal
if bufexists(fnamemodify("stream_internal.h", ":p")) | buffer stream_internal.h | else | edit stream_internal.h | endif
if &buftype ==# 'terminal'
  silent file stream_internal.h
endif
balt stream.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 16 - ((15 * winheight(0) + 15) / 30)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("stream.c", ":p")) | buffer stream.c | else | edit stream.c | endif
if &buftype ==# 'terminal'
  silent file stream.c
endif
balt stream_internal.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 24 - ((23 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 24
normal! 032|
wincmd w
exe '1resize ' . ((&lines * 30 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 78 + 104) / 209)
exe '2resize ' . ((&lines * 30 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 78 + 104) / 209)
exe 'vert 3resize ' . ((&columns * 130 + 104) / 209)
tabnext
edit format.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 35 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 70 + 104) / 209)
exe '2resize ' . ((&lines * 25 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 70 + 104) / 209)
exe '3resize ' . ((&lines * 37 + 32) / 64)
exe 'vert 3resize ' . ((&columns * 138 + 104) / 209)
exe '4resize ' . ((&lines * 23 + 32) / 64)
exe 'vert 4resize ' . ((&columns * 59 + 104) / 209)
exe '5resize ' . ((&lines * 23 + 32) / 64)
exe 'vert 5resize ' . ((&columns * 78 + 104) / 209)
argglobal
balt stream_internal.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 46 - ((14 * winheight(0) + 17) / 35)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 46
normal! 041|
wincmd w
argglobal
if bufexists(fnamemodify("format_internal.h", ":p")) | buffer format_internal.h | else | edit format_internal.h | endif
if &buftype ==# 'terminal'
  silent file format_internal.h
endif
balt format.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 12) / 25)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 012|
wincmd w
argglobal
if bufexists(fnamemodify("format.c", ":p")) | buffer format.c | else | edit format.c | endif
if &buftype ==# 'terminal'
  silent file format.c
endif
balt format.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 182 - ((16 * winheight(0) + 18) / 37)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 182
normal! 028|
wincmd w
argglobal
if bufexists(fnamemodify("format_funcs.h", ":p")) | buffer format_funcs.h | else | edit format_funcs.h | endif
if &buftype ==# 'terminal'
  silent file format_funcs.h
endif
balt List/List.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 32 - ((8 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 32
normal! 016|
wincmd w
argglobal
if bufexists(fnamemodify("format_funcs.c", ":p")) | buffer format_funcs.c | else | edit format_funcs.c | endif
if &buftype ==# 'terminal'
  silent file format_funcs.c
endif
balt List/List.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 202 - ((15 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 202
normal! 030|
wincmd w
exe '1resize ' . ((&lines * 35 + 32) / 64)
exe 'vert 1resize ' . ((&columns * 70 + 104) / 209)
exe '2resize ' . ((&lines * 25 + 32) / 64)
exe 'vert 2resize ' . ((&columns * 70 + 104) / 209)
exe '3resize ' . ((&lines * 37 + 32) / 64)
exe 'vert 3resize ' . ((&columns * 138 + 104) / 209)
exe '4resize ' . ((&lines * 23 + 32) / 64)
exe 'vert 4resize ' . ((&columns * 59 + 104) / 209)
exe '5resize ' . ((&lines * 23 + 32) / 64)
exe 'vert 5resize ' . ((&columns * 78 + 104) / 209)
tabnext
edit main.c
argglobal
balt format.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 11 - ((10 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 11
normal! 0
tabnext
edit List/List.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 98 + 104) / 209)
exe 'vert 2resize ' . ((&columns * 110 + 104) / 209)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 012|
lcd ~/projects/info
wincmd w
argglobal
if bufexists(fnamemodify("~/projects/info/List/List.c", ":p")) | buffer ~/projects/info/List/List.c | else | edit ~/projects/info/List/List.c | endif
if &buftype ==# 'terminal'
  silent file ~/projects/info/List/List.c
endif
balt ~/projects/info/List/List.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 53 - ((1 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 53
normal! 0
lcd ~/projects/info
wincmd w
exe 'vert 1resize ' . ((&columns * 98 + 104) / 209)
exe 'vert 2resize ' . ((&columns * 110 + 104) / 209)
tabnext 5
set stal=1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
