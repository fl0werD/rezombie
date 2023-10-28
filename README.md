# ReZombie

## What is this?
ReZombie is a modification for the Counter-Strike game based on the original Zombie Plague modification using modern technologies and deeper integration over the game library.

## Goals of the project
* Develop a modification architecture from scratch with the experience of previous years.
* Minimum dependencies between plugins.
* Extended API for plugins and minimum boilerplate code coverage.

## Requirements
ReHLDS, ReGameDLL, Metamod-r (or Metamod-P), AMX Mod X, ReAPI.
* Tip: Recommend using the latest versions.

## How can I help the project?
Just install it on your game server and report problems you faced.

## Acknowledgments
Thanks to projects: ReGameDLL_CS, AMX Mod X, ReAPI<br/>
Thanks to humans: wellasgood, Nvoymax, Alex, DANDY, steelzzz, Droads, Nordic Warrior, kostikovkirill, CaHTuK, Bodom, DimaS, bristol, wopox1337, PurposeLess and others.<br/>

Unlimited Player Classes & Sub-Classes<br/>
Weapons System (a wrapper for custom weapon based on default weapon) for weapons, knives and grenades.<br/>
Advanced Knives System for custom melee and knives<br/>
Grenade System (easy way to createEntities your own grenades)<br/>

## Вопрос - ответ
Вопрос: Неправильно работают хитбоксы (например: не попадает в голову, одинаковый урон).<br/>
Ответ: 
https://developer.valvesoftware.com/wiki/$hbox

## Цвета
Возможное использование
> CSSColorParser::parse(" rgba (255, 128, 12, 0.5)");
 Color [ 255, 128, 12, 0.5 ]
> CSSColorParser::parse("#fff");
 Color [ 255, 255, 255, 1 ]
> CSSColorParser::parse("#ff0011");
 Color [ 255, 0, 17, 1 ]
> CSSColorParser::parse("slateblue");
 Color [ 106, 90, 205, 1 ]
> CSSColorParser::parse("blah");
 Color [ 0, 0, 0, 1 ]
> CSSColorParser::parse("ffffff");
 Color [ 0, 0, 0, 1 ]