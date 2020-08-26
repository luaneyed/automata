# 3*4 Korean keyboard Simulator

## Environment
- gcc 4.7.1

## Keyboard arrangement
```
1 2 3
q w e
a s d
z x c
```
## Key mapping
```
1 : ㄱ
2 : ㄴ
3 : ㅏㅓ
q : ㄹ
w : ㅁ
e : ㅗㅜ
a : ㅅ
s : ㅇ
d : ㅣ
z : 획추가
x : ㅡ
c : 쌍자음
```

## Input Example
```
ㄱ : 1		ㄲ : 1c		ㄴ : 2		ㄷ : 2z		ㄸ : 2zc
ㄹ : q		ㅁ : w		ㅂ : wz		ㅃ : wzc		ㅅ : a
ㅆ : ac		ㅇ : s		ㅈ : az		ㅉ : azc		ㅊ : azz
ㅋ : 1zz		ㅌ : 2zz		ㅍ : wzz		ㅎ : sz		ㄳ : 1a
ㄵ : 2az		ㄶ : 2sz		ㄺ : q1		ㄻ : qw		ㄼ : qwz
ㄽ : qa		ㄾ : q2zz	ㄿ : qwzz	ㅀ : qsz		ㅄ : wza
ㅏ : 3		ㅐ : 3d		ㅑ : 3z		ㅒ : 3zd	
ㅓ : 33		ㅔ : 33d		ㅕ : 33z		ㅖ : 33zd
ㅗ : e		ㅘ : e3		ㅙ : e3d		ㅚ : ed		ㅛ : ez
ㅜ : ee		ㅝ : ee3		ㅞ : ee3d	ㅟ : eed		ㅠ : eez
ㅡ : x		ㅢ : xd		ㅣ : d
```

## Converted Regular Expression
- Initial consonant = `1+2+2z+q+w+wz+a+s+az+azz+1z+2zz+wzz+sz+1c+2zc+wzc+ac+azc`
- Vowel = `3+3d+3z+3zd+33+33d+33z+33zd+e+e3+e3d+ed+ez+ee+ee3+ee3d+eed+eez+x+xd+d`
- Final consonant = `1+2+2z+q+w+wz+a+s+az+azz+1z+2zz+wzz+sz+1c+ac+1a+2az+2sz+q1+qw+qwz+qa+q2zz+qwzz+qsz+wza`
- (character)* = `((initial)(vowel)(epsilon+final))*`
	= `((1+2+2z+q+w+wz+a+s+az+azz+1z+2zz+wzz+sz+1c+2zc+wzc+ac+azc)(3+3d+3z+3zd+33+33d+33z+33zd+e+e3+e3d+ed+ez+ee+ee3+ee3d+eed+eez+x+xd+d)($+1+2+2z+q+w+wz+a+s+az+azz+1z+2zz+wzz+sz+1c+ac+1a+2az+2sz+q1+qw+qwz+qa+q2zz+qwzz+qsz+wza))*`

## Backspace
`;`

## Input Example
- `1eedazz32szsxw` (귀찮음)

