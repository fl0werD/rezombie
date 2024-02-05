# ReZombie

## What is this?
ReZombie is a modification for the Counter-Strike game based on the original Zombie Plague modification using modern technologies and deeper integration over the game library.

Unlimited Player Classes & Sub-Classes<br/>
Weapons System (a wrapper for custom weapon based on default weapon) for weapons, knives and grenades.<br/>
Advanced Knives System for custom melee and knives<br/>
Grenade System (easy way to create your own grenades)<br/>

## Вопрос - ответ
Вопрос: Неправильно работают хитбоксы (например: не попадает в голову, одинаковый урон).<br/>
Ответ: 
https://developer.valvesoftware.com/wiki/$hbox

### Система цветов
Для AMXX API возможно использовать следующие шаблоны значений цветов:\
Цвета можно указывать следующими методами:
- Шестнадцатеричные цвета: `#fff`, `#ff0011`
- RGB цвета: `rgb(255, 128, 12)`
- RGBA цвета: `rgba(255, 128, 12, 1.0)`, `rgba(255, 128, 12, 100%)`

Будут интерпретированы как массив значений { 255, 128, 12, 255 }

**Предопределенные названия цветов**: можно указать конкретное имя цвета из списка [140 названий цветов](https://www.w3schools.com/cssref/css_colors.php)\
**Создание своих цветов цвета**: создать свой цвет и использовать его в других плагинах.<br>
create_color("название цвета", "значение цвета")

_Примечание_: alpha будет интерпретироваться как alpha, brightness или игнорироваться там, где нет возможности указать это значение.

## Acknowledgments
Thanks to projects: ReGameDLL_CS, AMX Mod X, ReAPI<br/>
Thanks to humans: wellasgood, Nvoymax, Alex, DANDY, steelzzz, Droads, Nordic Warrior, kostikovkirill, CaHTuK, Bodom, DimaS, bristol, wopox1337, PurposeLess and others.<br/>
