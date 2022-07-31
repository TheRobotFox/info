let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Desktop/Projekte/PATRISPREDICTUM/Projekte/Info
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
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
badd +15 info_internal.h
badd +81 format.c
badd +1 buffer.c
badd +1 stream.h
badd +1 format_internal.h
badd +1 config.h
badd +2 ANSI_internal.h
badd +1 List/List.c
badd +8 List/List.h
badd +13 format_funcs.h
badd +24 format_funcs.c
argglobal
%argdel
set stal=2
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
exe '1resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 1resize ' . ((&columns * 171 + 173) / 347)
exe '2resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 171 + 173) / 347)
exe 'vert 3resize ' . ((&columns * 175 + 173) / 347)
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
let s:l = 55 - ((0 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 55
normal! 0
wincmd w
argglobal
if bufexists("info_internal.h") | buffer info_internal.h | else | edit info_internal.h | endif
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
let s:l = 30 - ((29 * winheight(0) + 20) / 40)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 30
normal! 0117|
wincmd w
argglobal
if bufexists("info.c") | buffer info.c | else | edit info.c | endif
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
let s:l = 8 - ((7 * winheight(0) + 40) / 80)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 8
normal! 013|
wincmd w
exe '1resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 1resize ' . ((&columns * 171 + 173) / 347)
exe '2resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 171 + 173) / 347)
exe 'vert 3resize ' . ((&columns * 175 + 173) / 347)
tabnext
edit ANSI.h
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
exe 'vert 1resize ' . ((&columns * 172 + 173) / 347)
exe 'vert 2resize ' . ((&columns * 174 + 173) / 347)
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
let s:l = 6 - ((5 * winheight(0) + 40) / 80)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 02|
wincmd w
argglobal
if bufexists("ANSI.c") | buffer ANSI.c | else | edit ANSI.c | endif
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
let s:l = 17 - ((16 * winheight(0) + 40) / 80)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 17
normal! 08|
wincmd w
2wincmd w
exe 'vert 1resize ' . ((&columns * 172 + 173) / 347)
exe 'vert 2resize ' . ((&columns * 174 + 173) / 347)
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
exe 'vert 1resize ' . ((&columns * 172 + 173) / 347)
exe 'vert 2resize ' . ((&columns * 174 + 173) / 347)
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
let s:l = 8 - ((7 * winheight(0) + 40) / 80)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 8
normal! 066|
wincmd w
argglobal
if bufexists("buffer.c") | buffer buffer.c | else | edit buffer.c | endif
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
let s:l = 32 - ((0 * winheight(0) + 40) / 80)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 32
normal! 074|
wincmd w
exe 'vert 1resize ' . ((&columns * 172 + 173) / 347)
exe 'vert 2resize ' . ((&columns * 174 + 173) / 347)
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
exe '1resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 1resize ' . ((&columns * 171 + 173) / 347)
exe '2resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 171 + 173) / 347)
exe 'vert 3resize ' . ((&columns * 175 + 173) / 347)
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
let s:l = 1 - ((0 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 049|
wincmd w
argglobal
if bufexists("stream_internal.h") | buffer stream_internal.h | else | edit stream_internal.h | endif
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
let s:l = 2 - ((1 * winheight(0) + 20) / 40)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 2
normal! 016|
wincmd w
argglobal
if bufexists("stream.c") | buffer stream.c | else | edit stream.c | endif
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
let s:l = 17 - ((16 * winheight(0) + 40) / 80)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 17
normal! 060|
wincmd w
exe '1resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 1resize ' . ((&columns * 171 + 173) / 347)
exe '2resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 171 + 173) / 347)
exe 'vert 3resize ' . ((&columns * 175 + 173) / 347)
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
exe '1resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 1resize ' . ((&columns * 115 + 173) / 347)
exe '2resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 115 + 173) / 347)
exe '3resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 3resize ' . ((&columns * 231 + 173) / 347)
exe '4resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 4resize ' . ((&columns * 95 + 173) / 347)
exe '5resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 5resize ' . ((&columns * 135 + 173) / 347)
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
let s:l = 40 - ((21 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 40
normal! 040|
wincmd w
argglobal
if bufexists("format_internal.h") | buffer format_internal.h | else | edit format_internal.h | endif
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
let s:l = 29 - ((28 * winheight(0) + 20) / 40)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 29
normal! 0
wincmd w
argglobal
if bufexists("format.c") | buffer format.c | else | edit format.c | endif
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
let s:l = 178 - ((24 * winheight(0) + 20) / 40)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 178
normal! 027|
wincmd w
argglobal
if bufexists("format_funcs.h") | buffer format_funcs.h | else | edit format_funcs.h | endif
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
let s:l = 6 - ((5 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 030|
wincmd w
argglobal
if bufexists("format_funcs.c") | buffer format_funcs.c | else | edit format_funcs.c | endif
if &buftype ==# 'terminal'
  silent file format_funcs.c
endif
balt info_internal.h
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
let s:l = 30 - ((29 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 30
normal! 078|
wincmd w
exe '1resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 1resize ' . ((&columns * 115 + 173) / 347)
exe '2resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 115 + 173) / 347)
exe '3resize ' . ((&lines * 40 + 41) / 83)
exe 'vert 3resize ' . ((&columns * 231 + 173) / 347)
exe '4resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 4resize ' . ((&columns * 95 + 173) / 347)
exe '5resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 5resize ' . ((&columns * 135 + 173) / 347)
tabnext 2
set stal=1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToOFAc
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
