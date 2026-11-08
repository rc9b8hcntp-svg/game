#pragma once
#include <Arduino.h>

// Colle ton HTML ENTRE les deux lignes ci-dessous :
// de <!DOCTYPE html> jusqu'à </html> (sans rien ajouter ni retirer).
const char HTML_GAME[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<!--
  ============================================================================
  CONSIGNE POUR CLAUDE (ou toute autre IA / personne qui modifie ce fichier)
  ============================================================================
  À CHAQUE FOIS que tu fais une mise à jour de ce jeu (nouvelle fonction, correction, changement de menu…),
  AJOUTE AUTOMATIQUEMENT une entrée dans les NEWS du jeu, sans que l'utilisateur ait à le demander :
    1. Cherche « const NEWS=[ » (le commentaire « NEWS : à CHAQUE mise à jour… » est juste au-dessus).
    2. Ajoute EN PREMIER (tout en haut de la liste) une ligne de ce type :
         {d:'JJ/MM/AAAA',t:'Titre court',b:'Une ou deux phrases qui expliquent ce qui a changé pour le joueur.'},
       (optionnel : go:['lobby','Texte du bouton'] pour ajouter un bouton qui ouvre un écran du jeu)
    3. Utilise la date du jour. Échappe les apostrophes dans le texte avec \' (exemple : l\'accueil).
    4. Dis à l'utilisateur, en une phrase, que l'entrée NEWS a été ajoutée.
  Ne supprime pas les anciennes entrées à la main : le jeu efface tout seul les plus anciennes au-delà de NEWS_MAX (10) juste après la liste. Ne mets pas d'entrée pour un changement invisible pour le joueur.
  ============================================================================
-->
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<title>Rocket Arena 3D</title>
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Rajdhani:wght@600;700&display=swap">
<style>
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent;-webkit-user-select:none;user-select:none}
html,body{touch-action:pan-x pan-y;margin:0;height:100%;overflow:hidden;background:#05070f;color:#fff;font-family:system-ui,-apple-system,"Segoe UI",Roboto,Arial,sans-serif}
#c{position:fixed;inset:0;width:100%;height:100%;display:block;touch-action:none}
.hide{display:none!important}
#hud{position:fixed;inset:0;pointer-events:none;z-index:5}
#score{position:absolute;top:max(8px,env(safe-area-inset-top));left:50%;transform:translateX(-50%);display:flex;align-items:stretch;border-radius:12px;overflow:hidden;box-shadow:0 4px 24px rgba(0,0,0,.5);font-weight:900;font-style:italic}
.tm{display:flex;align-items:center;gap:10px;padding:4px 16px;font-size:clamp(12px,2.4vmin,18px);letter-spacing:1px}
.tm i{font-style:italic;font-size:clamp(24px,6vmin,44px);min-width:1ch;text-align:center}
.tb{background:linear-gradient(90deg,rgba(20,70,170,.92),rgba(47,139,255,.78))}
.to{background:linear-gradient(90deg,rgba(255,138,42,.78),rgba(170,70,10,.92))}
#grHud{position:absolute;top:calc(max(8px,env(safe-area-inset-top)) + 58px);left:50%;transform:translateX(-50%);text-align:center;font-weight:800;color:#fff;text-shadow:0 2px 8px #000;letter-spacing:1px;pointer-events:none;white-space:nowrap}
#grHud b{display:block;font-size:22px;font-style:italic}
#grHud small{display:block;font-size:12px;opacity:.8;font-weight:600;margin-top:2px}
#grHud .pips{display:flex;gap:5px;justify-content:center;margin-top:5px}
#grHud .pips i{width:18px;height:6px;border-radius:3px;background:rgba(255,255,255,.25)}
#grHud .pips i.d{background:#8794ad}
#grHud .pips i.a{background:#35ff9a;box-shadow:0 0 8px #35ff9a}
#grHud .pips i.r{background:#ffd23f;box-shadow:0 0 8px #ffd23f}
#clockBox{background:rgba(5,8,20,.9);padding:4px 16px;text-align:center;min-width:92px}
#clock{font-size:clamp(20px,5vmin,34px);font-variant-numeric:tabular-nums}
#ot{font-size:11px;color:#ffd166;letter-spacing:2px;min-height:1em}
#boost{position:absolute;right:max(14px,env(safe-area-inset-right));bottom:max(14px,env(safe-area-inset-bottom));text-align:center}
body.touch #boost{right:auto;left:max(12px,env(safe-area-inset-left));top:max(10px,env(safe-area-inset-top));bottom:auto}
#ring{--p:33;width:clamp(70px,16vmin,120px);height:clamp(70px,16vmin,120px);border-radius:50%;background:conic-gradient(#ffb02e calc(var(--p)*1%),rgba(0,0,0,.55) 0);display:flex;align-items:center;justify-content:center;box-shadow:0 0 24px rgba(255,150,30,.35)}
#bnum{width:78%;height:78%;border-radius:50%;background:rgba(6,10,24,.92);display:flex;align-items:center;justify-content:center;font-size:clamp(22px,6vmin,40px);font-weight:900;font-style:italic}
#spd{font-size:12px;opacity:.8;margin-top:4px}
#feed{position:absolute;right:12px;top:70px;display:flex;flex-direction:column;align-items:flex-end;gap:4px;font-size:13px;font-weight:700}
#feed div{background:rgba(6,10,24,.78);padding:4px 10px;border-radius:8px;border-left:3px solid #fff}
#msg{position:fixed;inset:0;z-index:6;pointer-events:none;display:flex;flex-direction:column;align-items:center;justify-content:center;text-align:center}
#mbig{font-size:clamp(48px,16vmin,150px);font-weight:900;font-style:italic;letter-spacing:2px;text-shadow:0 0 30px currentColor,0 6px 0 rgba(0,0,0,.4)}
#msub{font-size:clamp(16px,4vmin,34px);font-weight:800;margin-top:6px;text-shadow:0 2px 8px #000}
.pop{animation:pop .5s cubic-bezier(.2,1.6,.4,1)}
@keyframes pop{from{transform:scale(2.2);opacity:0}to{transform:scale(1);opacity:1}}
#flash{position:fixed;inset:0;background:#fff;opacity:0;pointer-events:none;z-index:7;transition:opacity .6s}
#rtag{position:fixed;top:14%;left:50%;transform:translateX(-50%);z-index:8;font-weight:900;font-style:italic;font-size:clamp(20px,5vmin,36px);letter-spacing:4px;text-align:center;text-shadow:0 0 20px #4af;pointer-events:none}
#rtag small{display:block;font-size:12px;letter-spacing:1px;opacity:.7;font-style:normal;font-weight:600}
#touch{position:fixed;inset:0;z-index:9;pointer-events:none}
#joyZone{position:absolute;left:0;bottom:0;width:48%;height:72%;pointer-events:auto;touch-action:none}
#joyBase{position:absolute;width:22vmin;height:22vmin;margin:-11vmin 0 0 -11vmin;border-radius:50%;border:2px solid rgba(255,255,255,.35);background:rgba(255,255,255,.08);display:none;pointer-events:none}
#joyKnob{position:absolute;left:50%;top:50%;width:10vmin;height:10vmin;margin:-5vmin 0 0 -5vmin;border-radius:50%;background:rgba(255,255,255,.45)}
.btn{position:absolute;pointer-events:auto;touch-action:none;display:flex;align-items:center;justify-content:center;border-radius:50%;font-weight:900;font-size:clamp(11px,3vmin,18px);letter-spacing:1px;border:3px solid rgba(255,255,255,.45);background:rgba(20,30,60,.55);color:#fff}
.btn.down{background:rgba(80,150,255,.8);transform:scale(.94)}
#bJump{right:max(3vmin,env(safe-area-inset-right));bottom:max(5vmin,env(safe-area-inset-bottom));width:23vmin;height:23vmin;background:rgba(40,120,255,.5)}
#bBoost{right:calc(max(3vmin,env(safe-area-inset-right)) + 25vmin);bottom:max(3vmin,env(safe-area-inset-bottom));width:20vmin;height:20vmin;background:rgba(255,140,30,.5)}
#bDrift{right:max(3vmin,env(safe-area-inset-right));bottom:calc(max(5vmin,env(safe-area-inset-bottom)) + 26vmin);width:15vmin;height:15vmin}
#bCam{right:calc(max(3vmin,env(safe-area-inset-right)) + 26vmin);bottom:calc(max(3vmin,env(safe-area-inset-bottom)) + 22vmin);width:11vmin;height:11vmin}
#bPause{right:max(10px,env(safe-area-inset-right));top:max(10px,env(safe-area-inset-top));width:9vmin;height:9vmin;min-width:38px;min-height:38px}
.ov{position:fixed;inset:0;display:none;z-index:30;align-items:center;justify-content:center;background:rgba(3,6,16,.5);backdrop-filter:blur(2px)}
.ov.show{display:flex}
.panel{width:min(94vw,660px);max-height:94vh;overflow:auto;touch-action:pan-y;padding:18px 20px 16px;border-radius:18px;background:linear-gradient(160deg,rgba(14,22,48,.93),rgba(8,12,28,.93));border:1px solid rgba(120,170,255,.28);box-shadow:0 10px 60px rgba(0,0,0,.65)}
.panel.small{width:min(90vw,380px);text-align:center}
h1{margin:0;font-size:clamp(28px,7vw,48px);font-style:italic;font-weight:900;letter-spacing:1px;background:linear-gradient(90deg,#2f8bff,#fff 50%,#ff8a2a);-webkit-background-clip:text;background-clip:text;color:transparent}
h2{margin:0 0 8px;font-size:clamp(24px,6vw,36px);font-style:italic;font-weight:900}
.sub{opacity:.7;font-size:13px;margin:2px 0 8px}
.grp{margin:8px 0}.grp>b{display:block;font-size:10px;letter-spacing:2px;opacity:.6;margin-bottom:4px}
.opts{display:flex;gap:6px;flex-wrap:wrap}
.opt{flex:1;min-width:58px;padding:9px 6px;border-radius:10px;border:1px solid rgba(255,255,255,.16);background:rgba(255,255,255,.06);color:#fff;font-weight:700;font-size:13px;font-family:inherit;text-align:center;cursor:pointer}
.opt.on{background:linear-gradient(180deg,#3d8bff,#1b56c8);border-color:#a8cbff;box-shadow:0 0 14px rgba(60,140,255,.5)}
.go{display:block;width:100%;margin-top:12px;padding:14px;border:0;border-radius:12px;font-weight:900;font-style:italic;font-size:20px;font-family:inherit;letter-spacing:2px;color:#fff;background:linear-gradient(90deg,#ff7a1a,#ff4b2b);cursor:pointer;box-shadow:0 6px 24px rgba(255,100,30,.4)}
.go.alt{background:linear-gradient(90deg,#2a3a66,#1c2748);box-shadow:none;font-size:16px;padding:11px}
.help{font-size:11.5px;opacity:.65;line-height:1.5;margin-top:6px}
table{width:100%;border-collapse:collapse;font-size:13px;margin-top:8px}
th{opacity:.6;font-size:10px;letter-spacing:1px;text-align:right;padding:3px 6px}
td{padding:5px 6px;text-align:right;border-top:1px solid rgba(255,255,255,.08)}
th:first-child,td:first-child{text-align:left}
.tb2{color:#6db0ff}.to2{color:#ffa45c}
/* --- popups de stats façon Rocket League --- */
#pops{position:absolute;left:50%;top:62%;transform:translateX(-50%);display:flex;flex-direction:column;align-items:center;gap:2px;pointer-events:none}
#pops div{font-weight:900;font-style:italic;font-size:clamp(14px,3.2vmin,26px);letter-spacing:2px;text-shadow:0 2px 10px #000;animation:pp 2.2s ease-out forwards;white-space:nowrap}
#pops div b{color:#ffd166;margin-left:10px}
@keyframes pp{0%{transform:translateY(14px) scale(1.4);opacity:0}12%{transform:none;opacity:1}75%{opacity:1}100%{transform:translateY(-22px);opacity:0}}
#sb{position:fixed;inset:0;z-index:12;display:none;align-items:center;justify-content:center;pointer-events:none}
#sb.show{display:flex}
#sb .panel{pointer-events:none;width:min(94vw,640px)}
#sprint{position:absolute;left:50%;bottom:calc(max(14px,env(safe-area-inset-bottom)) + 4px);transform:translateX(-50%);font-weight:900;font-style:italic;letter-spacing:3px;font-size:clamp(12px,2.6vmin,20px);color:#ffd166;text-shadow:0 0 14px #ff9a30;opacity:0;transition:opacity .2s}
#vig{position:fixed;inset:0;pointer-events:none;z-index:4;background:radial-gradient(ellipse at center,rgba(0,0,0,0) 55%,rgba(0,0,0,.55) 100%);opacity:.7}
#spdl{position:fixed;inset:0;pointer-events:none;z-index:4;opacity:0;background:radial-gradient(ellipse at center,rgba(255,255,255,0) 45%,rgba(120,190,255,.22) 100%);transition:opacity .15s}
/* --- interface lobby / casier / boutique --- */
#ui{position:fixed;inset:0;z-index:20;display:none;pointer-events:none}
#ui.show{display:block}
#ui button{font-family:inherit;color:#fff;cursor:pointer}
#topbar{position:absolute;left:0;right:0;top:0;display:flex;align-items:center;gap:10px;padding:max(8px,env(safe-area-inset-top)) 12px 8px;background:linear-gradient(180deg,rgba(4,7,18,.92),rgba(4,7,18,0));pointer-events:auto;flex-wrap:wrap}
#prof{display:flex;flex-direction:column;justify-content:center;min-width:150px;padding:6px 12px;border-radius:10px;border:1px solid rgba(255,255,255,.25);box-shadow:0 2px 12px rgba(0,0,0,.4)}
#pname{font-weight:900;font-style:italic;font-size:15px;text-shadow:0 1px 4px #000}
#ptitle{font-size:10px;opacity:.85;letter-spacing:1px;text-shadow:0 1px 3px #000}
#lvl{font-size:10px;font-weight:800;margin-top:2px;text-shadow:0 1px 3px #000}
.xpb{height:4px;border-radius:2px;background:rgba(0,0,0,.45);margin-top:3px;overflow:hidden}
#xpbar{height:100%;width:0;background:linear-gradient(90deg,#ffb02e,#ffe08a)}
#nav{display:flex;gap:4px;flex:1;flex-wrap:wrap}
.nb{padding:9px 14px;border:0;border-radius:8px;background:rgba(255,255,255,.07);font-weight:800;font-size:13px;letter-spacing:1px}
.nb.on{background:linear-gradient(180deg,#3d8bff,#1b56c8);box-shadow:0 0 14px rgba(60,140,255,.5)}
#wallet{display:flex;align-items:center;gap:8px}
#cred{font-weight:900;font-size:14px;background:rgba(0,0,0,.5);padding:8px 12px;border-radius:8px;border:1px solid rgba(255,200,80,.35)}
#daily{padding:8px 12px;border:0;border-radius:8px;background:rgba(255,255,255,.1);font-weight:800;font-size:13px}
#daily.pulse{background:linear-gradient(90deg,#ff7a1a,#ff4b2b);animation:pl 1.2s infinite}
@keyframes pl{50%{box-shadow:0 0 18px rgba(255,120,40,.9)}}
#scr{position:absolute;left:0;right:0;top:64px;bottom:0;pointer-events:none}
.rpanel,.cpanel{pointer-events:auto;background:linear-gradient(160deg,rgba(14,22,48,.9),rgba(8,12,28,.92));border:1px solid rgba(120,170,255,.28);border-radius:16px;box-shadow:0 10px 50px rgba(0,0,0,.6)}
.rpanel{position:absolute;right:max(10px,env(safe-area-inset-right));top:6px;bottom:max(10px,env(safe-area-inset-bottom));width:min(52vw,560px);display:flex;flex-direction:column;overflow:hidden}
.cpanel{position:absolute;left:50%;top:6px;transform:translateX(-50%);width:min(94vw,720px);max-height:calc(100% - 16px);overflow:auto;padding:16px 18px;touch-action:pan-y}
.cpanel.wide{width:min(96vw,920px)}
.phead{padding:12px 16px 4px}
.phead h2,.cpanel h2{margin:0 0 4px}
.pbody{flex:1;overflow:auto;padding:8px 14px;touch-action:pan-y;-webkit-overflow-scrolling:touch}
.pfoot{padding:10px 14px;border-top:1px solid rgba(255,255,255,.1);background:rgba(0,0,0,.25)}
h3{margin:14px 0 6px;font-size:13px;letter-spacing:2px;opacity:.85}
h3 small{color:#7dffb0;margin-left:6px}
.tabs{display:flex;gap:4px;padding:8px 10px 0;overflow-x:auto;flex:none;scrollbar-width:thin}
.tab{flex:none;padding:9px 12px;border:0;border-radius:8px 8px 0 0;background:rgba(255,255,255,.06);font-weight:700;font-size:12px;white-space:nowrap}
.tab.on{background:rgba(61,139,255,.35);box-shadow:inset 0 -3px 0 #4aa0ff}
.modes{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:8px}
.mode{display:flex;flex-direction:column;align-items:flex-start;gap:2px;text-align:left;padding:10px 12px;border-radius:12px;border:1px solid rgba(255,255,255,.14);background:rgba(255,255,255,.05)}
.mode i{font-style:normal;font-size:22px}
.mode b{font-size:14px}
.mode small{font-size:10.5px;opacity:.65;line-height:1.35}
.mode.on{background:linear-gradient(160deg,rgba(61,139,255,.45),rgba(27,86,200,.35));border-color:#a8cbff;box-shadow:0 0 16px rgba(60,140,255,.4)}
.go.sm{display:inline-block;width:auto;margin:0;padding:10px 16px;font-size:15px;letter-spacing:1px}
.go[disabled]{opacity:.45;pointer-events:none}
.pfoot .go{margin-top:0}
#lobinfo{position:absolute;left:max(16px,env(safe-area-inset-left));bottom:18px;display:flex;flex-direction:column;font-size:20px;font-weight:900;font-style:italic;text-shadow:0 2px 10px #000;pointer-events:none}
#lobinfo small{font-size:12px;opacity:.8;font-style:normal;letter-spacing:1px}
.rank{display:inline-flex;flex-direction:column;padding:8px 12px;border:2px solid;border-radius:10px;background:rgba(0,0,0,.35);margin:6px 6px 0 0;font-weight:900}
.rank small{font-size:10px;opacity:.7;font-weight:600}
.ranks{display:flex;flex-wrap:wrap}
.lbt{width:100%;border-collapse:collapse;margin-top:8px;font-size:15px}
.lbt th{text-align:left;font-size:11px;letter-spacing:2px;opacity:.6;padding:4px 8px}
.lbt td{padding:7px 8px;border-top:1px solid rgba(255,255,255,.08)}
.lbt td.n{width:46px;font-weight:900;font-style:italic;opacity:.85}
.lbt td.v{text-align:right;font-weight:900;white-space:nowrap}
.lbt small{opacity:.6;margin-left:6px;font-weight:600}
.lbt tr.me td{background:rgba(255,176,46,.16);border-top-color:rgba(255,176,46,.5)}
.lbt tr.t1 td.n{color:#ffd166}.lbt tr.t2 td.n{color:#c0c8d4}.lbt tr.t3 td.n{color:#d99a5a}
.lbme{margin:10px 0 2px;padding:10px 12px;border-left:4px solid #ffb02e;background:rgba(0,0,0,.35);font-weight:800;font-size:15px}
.lbmsg{padding:18px 6px;opacity:.8;text-align:center;font-size:15px}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(98px,1fr));gap:8px;margin:6px 0}
.card{display:flex;flex-direction:column;gap:2px;padding:6px;border-radius:10px;border:1px solid rgba(255,255,255,.12);border-bottom:3px solid var(--rc,#888);background:rgba(255,255,255,.05);text-align:center;min-width:0}
.card b{font-size:11px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.card small{font-size:10px;opacity:.75}
.card .cv{width:100%;aspect-ratio:200/124;object-fit:cover;border-radius:6px;background:rgba(0,0,0,.35)}
.card .cv.tt{display:flex;align-items:center;justify-content:center;font-size:11px;font-weight:800;padding:4px;color:#ffd166}
div.cv{display:flex;align-items:center;justify-content:center;border-radius:6px;min-height:52px}
div.cv b{font-size:12px;text-shadow:0 1px 4px #000}
.card.lock{opacity:.62}
.card.eq{box-shadow:0 0 0 2px #7dffb0 inset}
.card.sel{box-shadow:0 0 0 2px #ffd166 inset;transform:translateY(-2px)}
.frow{display:flex;align-items:center;gap:10px;margin:4px 0;flex-wrap:wrap}
.finfo{flex:1;min-width:120px;font-size:13px}
.finfo small{opacity:.7}
.okk{color:#7dffb0;font-weight:800;font-size:13px}
.sw{display:flex;flex-wrap:wrap;gap:6px}
.swb{width:34px;height:34px;border-radius:50%;border:2px solid rgba(255,255,255,.3)}
.swb.on{border-color:#fff;box-shadow:0 0 10px #fff}
.chips{display:flex;flex-wrap:wrap;gap:5px;margin:6px 0}
.chip2{padding:6px 10px;border:1px solid rgba(255,255,255,.16);border-radius:16px;background:rgba(255,255,255,.06);font-size:12px;font-weight:700}
.chip2.on{background:#2b6fe0;border-color:#a8cbff}
.crates{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:10px;margin-top:8px}
.crate{display:flex;flex-direction:column;align-items:center;gap:4px;text-align:center;padding:12px;border-radius:14px;border:2px solid var(--rc);background:rgba(0,0,0,.3)}
.cbox{font-size:46px;filter:drop-shadow(0 0 10px var(--rc))}
.crate small{font-size:11px;opacity:.7}
.bund{display:flex;align-items:center;gap:12px;justify-content:space-between;padding:10px;margin:8px 0;border-radius:12px;background:rgba(255,255,255,.05);border:1px solid rgba(255,255,255,.12)}
.bi{display:flex;flex-wrap:wrap;gap:4px;margin-top:6px}
.bchip{font-size:10.5px;padding:3px 7px;border:1px solid;border-radius:10px;background:rgba(0,0,0,.3)}
.reveal{display:flex;flex-direction:column;align-items:center;gap:4px;padding:14px;margin:8px 0;border-radius:14px;border:2px solid var(--rc);box-shadow:0 0 30px var(--rc)}
.reveal .cv{width:220px;max-width:70vw;aspect-ratio:200/124;border-radius:8px;background:rgba(0,0,0,.4)}
.bar{height:8px;border-radius:4px;background:rgba(0,0,0,.45);overflow:hidden;margin:5px 0}
.bar i{display:block;height:100%;background:linear-gradient(90deg,#3d8bff,#7dffb0)}
.bar.big{height:14px}
.grid.pass{grid-template-columns:repeat(auto-fill,minmax(84px,1fr))}
.pcard{position:relative;display:flex;flex-direction:column;align-items:center;gap:2px;padding:8px 4px;border-radius:10px;border:1px solid rgba(255,255,255,.12);border-bottom:3px solid var(--rc,#ffb02e);background:rgba(255,255,255,.05);font-size:10px;text-align:center}
.pcard b{font-size:20px}
.pcard.done{opacity:.55}
.pcard em{position:absolute;top:2px;right:5px;color:#7dffb0;font-style:normal}
.chal{display:flex;gap:12px;align-items:center;justify-content:space-between;padding:10px;margin:8px 0;border-radius:12px;background:rgba(255,255,255,.05)}
.chal>div{flex:1}
.chal small{opacity:.7}
.stats{display:grid;grid-template-columns:repeat(4,1fr);gap:8px}
.stats div{display:flex;flex-direction:column;align-items:center;padding:8px;border-radius:10px;background:rgba(255,255,255,.05)}
.stats b{font-size:20px}.stats small{font-size:10px;opacity:.7}
.txt{width:100%;padding:10px;border-radius:8px;border:1px solid rgba(255,255,255,.2);background:rgba(0,0,0,.4);color:#fff;font-size:15px;-webkit-user-select:text;user-select:text}
.bracket{display:flex;gap:14px;margin:12px 0;overflow-x:auto}
.bcol{flex:1;min-width:170px;display:flex;flex-direction:column;justify-content:space-around;gap:10px}
.bcol>small{opacity:.7;letter-spacing:1px;font-size:11px;text-align:center}
.bm{display:flex;flex-direction:column;gap:3px;padding:6px;border-radius:10px;background:rgba(255,255,255,.05)}
.bt{padding:7px 10px;border-radius:7px;background:rgba(0,0,0,.4);font-size:12px;font-weight:700}
.bt.me{border-left:3px solid #4aa0ff}
.bt.win{background:rgba(60,200,120,.3)}
.bt.out{opacity:.4;text-decoration:line-through}
#queue,#modal{position:fixed;inset:0;z-index:40;display:none;align-items:center;justify-content:center;background:rgba(3,6,16,.7);backdrop-filter:blur(3px);pointer-events:auto}
#queue.show,#modal.show{display:flex}
#qbox,#mbox{width:min(92vw,440px);max-height:90vh;overflow:auto;text-align:center;padding:22px;border-radius:18px;background:linear-gradient(160deg,rgba(14,22,48,.97),rgba(8,12,28,.97));border:1px solid rgba(120,170,255,.35)}
.spin{width:54px;height:54px;margin:14px auto;border-radius:50%;border:5px solid rgba(255,255,255,.15);border-top-color:#4aa0ff;animation:sp 1s linear infinite}
@keyframes sp{to{transform:rotate(360deg)}}
.vs{display:flex;align-items:center;justify-content:center;gap:12px;margin:12px 0;font-size:15px;flex-wrap:wrap}
.vs span{font-weight:900;font-style:italic;opacity:.6}
#toast{position:fixed;left:50%;bottom:24px;transform:translate(-50%,20px);z-index:60;padding:10px 18px;border-radius:20px;background:rgba(10,16,36,.95);border:1px solid rgba(120,170,255,.45);font-weight:700;font-size:14px;opacity:0;pointer-events:none;transition:.25s}
#toast.show{opacity:1;transform:translate(-50%,0)}
#pw{position:absolute;left:50%;bottom:calc(max(14px,env(safe-area-inset-bottom)) + 40px);transform:translateX(-50%);display:none;padding:10px 18px;border-radius:24px;background:rgba(10,16,36,.85);border:2px solid #ffd166;font-weight:900;font-style:italic;letter-spacing:1px;font-size:clamp(13px,2.8vmin,20px);color:#ffd166;box-shadow:0 0 18px rgba(255,209,102,.5)}
#bPower{right:calc(max(3vmin,env(safe-area-inset-right)) + 26vmin);bottom:calc(max(3vmin,env(safe-area-inset-bottom)) + 24vmin);width:15vmin;height:15vmin;background:rgba(255,209,102,.45);font-size:clamp(9px,2.4vmin,14px)}
.rew{display:flex;flex-wrap:wrap;gap:8px;justify-content:center;margin:10px 0}
.rew span{padding:6px 12px;border-radius:14px;background:rgba(255,255,255,.08);font-weight:800;font-size:13px}
.rew .up{background:rgba(125,255,176,.18);color:#7dffb0}
@media(max-width:760px) and (min-height:521px){
  .rpanel{width:calc(100vw - 20px);top:auto;height:56%;bottom:max(8px,env(safe-area-inset-bottom))}
  #lobinfo{bottom:auto;top:70px}
  .nb{padding:8px 9px;font-size:11px}
  #prof{min-width:120px}
  #scr{top:56px}
  .stats{grid-template-columns:repeat(2,1fr)}
}
#mv{position:absolute;left:max(10px,env(safe-area-inset-left));top:74px;width:min(340px,44vw);padding:10px 12px;border-radius:14px;background:linear-gradient(160deg,rgba(14,22,48,.86),rgba(8,12,28,.88));border:1px solid rgba(120,170,255,.3);box-shadow:0 8px 30px rgba(0,0,0,.5);font-size:12px;line-height:1.4;max-height:calc(100vh - 90px);overflow:auto}
body.touch #mv{top:96px;width:min(300px,40vw);font-size:11px}
#mvBar{display:flex;align-items:center;justify-content:space-between;gap:8px;margin-bottom:6px}
#mvS{display:inline-block;font-weight:900;font-size:11px;letter-spacing:1.5px;padding:3px 8px;border-radius:8px}
#mvMin{pointer-events:auto;cursor:pointer;flex:none;width:28px;height:24px;line-height:1;border-radius:8px;border:1px solid rgba(255,255,255,.25);background:rgba(255,255,255,.1);color:#fff;font:900 16px inherit;font-family:inherit;padding:0}
#mvMin:hover{background:rgba(61,139,255,.45)}
#mv.min{width:auto;padding:6px 8px;overflow:visible}
#mv.min #mvBar{margin-bottom:0}
#mv.min #mvBody{display:none}
#mvS.demo{background:#ff7a1a;color:#fff}
#mvS.try{background:#2fbf71;color:#04210f}
#mvT{font-size:17px;font-weight:900;font-style:italic}
#mvT small{font-size:10px;font-weight:700;font-style:normal;opacity:.7;letter-spacing:1px}
#mvD{opacity:.85;margin:4px 0}
#mvL{margin:4px 0 6px;padding-left:18px;opacity:.9}
#mvL li{margin:2px 0}
#mvG{font-weight:800;color:#ffd166;margin:4px 0}
#mvC{opacity:.8;margin-bottom:6px}
#mvB{display:flex;flex-wrap:wrap;gap:5px;pointer-events:auto}
#mvB button{pointer-events:auto;flex:1;min-width:46%;padding:6px 4px;border-radius:8px;border:1px solid rgba(255,255,255,.2);background:rgba(255,255,255,.08);color:#fff;font:700 11px inherit;font-family:inherit;cursor:pointer}
#mvB button:hover{background:rgba(61,139,255,.4)}
.mvcard{margin-top:10px;padding:10px 12px;border-radius:12px;background:rgba(0,0,0,.28);border:1px solid rgba(255,255,255,.12);font-size:12.5px;line-height:1.45}
.mvcard b{font-size:15px}.mvcard em{font-size:10px;font-style:normal;letter-spacing:1px;background:#ff7a1a;border-radius:6px;padding:1px 6px;margin-left:6px;vertical-align:middle}
.mvcard p{margin:6px 0;opacity:.9}.mvcard ol{margin:4px 0 6px;padding-left:20px}.mvcard li{margin:2px 0}
.mvcard .mvg{font-weight:800;color:#ffd166}.mvcard small{display:block;margin-top:4px;opacity:.7}


/* ===== MENU façon Rocket League : navigation centrée, tuiles obliques, accents orange/bleu ===== */
#ui,#ui button,.panel,.go{font-family:"Rajdhani","Saira Condensed","Segoe UI",system-ui,sans-serif}
#ui.show::before{content:"";position:absolute;inset:0;pointer-events:none;background:
 radial-gradient(120% 90% at 30% 55%,rgba(30,70,160,.0) 35%,rgba(2,4,14,.78) 100%),
 linear-gradient(115deg,rgba(40,110,255,.16) 0 18%,transparent 18% 22%,rgba(255,130,30,.10) 22% 30%,transparent 30%)}
#topbar{background:linear-gradient(180deg,rgba(4,7,18,.95),rgba(4,7,18,.0));gap:16px;align-items:flex-start}
#prof{border:0;border-left:5px solid #ff8a2a;border-radius:0;clip-path:polygon(0 0,100% 0,calc(100% - 14px) 100%,0 100%);padding:6px 26px 6px 12px;min-width:190px}
#pname{font-size:20px;letter-spacing:1px;text-transform:uppercase}
#nav{justify-content:center;gap:2px;align-self:center}
.nb{background:transparent;border-radius:0;font-style:italic;font-weight:700;font-size:19px;letter-spacing:2px;padding:8px 18px;color:rgba(255,255,255,.62);position:relative;transition:color .15s,transform .15s}
.nb:hover{color:#fff;transform:translateY(-1px)}
.nb.on{background:transparent;box-shadow:none;color:#fff;text-shadow:0 0 14px rgba(90,170,255,.9)}
.nb.on::after{content:"";position:absolute;left:12px;right:12px;bottom:0;height:4px;background:linear-gradient(90deg,#2f8bff,#fff,#ff8a2a);transform:skewX(-30deg)}
#cred{border-radius:0;clip-path:polygon(12px 0,100% 0,calc(100% - 12px) 100%,0 100%);padding:8px 22px;font-size:17px;font-style:italic}
#daily{border-radius:0;font-weight:700;font-style:italic;letter-spacing:1px;text-transform:uppercase}
.rpanel{background:linear-gradient(180deg,rgba(8,16,44,.80),rgba(4,8,24,.9));border:0;border-top:3px solid #2f8bff;border-radius:0;box-shadow:0 0 60px rgba(0,0,0,.7)}
.phead h2{font-size:clamp(30px,5vw,44px);letter-spacing:3px;text-transform:uppercase}
.phead .sub{text-transform:uppercase;letter-spacing:2px;font-size:12px}
.modes{display:flex;flex-direction:column;gap:6px}
.mode{flex-direction:row;align-items:center;gap:14px;width:100%;padding:10px 34px 10px 16px;border:0;border-left:5px solid rgba(255,255,255,.25);border-radius:0;
 background:linear-gradient(90deg,rgba(255,255,255,.09),rgba(255,255,255,.02));clip-path:polygon(0 0,100% 0,calc(100% - 18px) 100%,0 100%);color:#fff;transition:transform .12s,background .12s}
.mode:hover{transform:translateX(4px);background:linear-gradient(90deg,rgba(255,255,255,.16),rgba(255,255,255,.05))}
.mode i{font-size:26px;width:34px;text-align:center}
.mode b{font-size:20px;font-style:italic;text-transform:uppercase;letter-spacing:1.5px;flex:none;min-width:140px}
.mode small{font-size:13px;opacity:.7;flex:1}
.mode.on{background:linear-gradient(90deg,#ffffff,#bcd9ff);border-left-color:#ff8a2a;box-shadow:none;color:#0a1840}
.mode.on small{opacity:.8;color:#0a1840}
.opt{border-radius:0;clip-path:polygon(8px 0,100% 0,calc(100% - 8px) 100%,0 100%);border:0;background:rgba(255,255,255,.1);font-style:italic;text-transform:uppercase;font-size:14px;letter-spacing:1px}
.opt.on{background:linear-gradient(180deg,#ffffff,#bcd9ff);color:#0a1840;box-shadow:none}
.grp>b{font-size:12px;letter-spacing:3px;opacity:.75}
.go{border-radius:0;clip-path:polygon(18px 0,100% 0,calc(100% - 18px) 100%,0 100%);background:linear-gradient(90deg,#ff7a1a,#ffb02e);color:#1a0d00;font-size:26px;letter-spacing:4px;box-shadow:none;transition:filter .15s,transform .15s}
.go:hover{filter:brightness(1.12);transform:scale(1.01)}
.go.alt{background:linear-gradient(90deg,#2a3a66,#1c2748);color:#fff;font-size:18px}
.panel{border-radius:0;border-top:3px solid #2f8bff}
#lobinfo{font-size:26px;text-transform:uppercase;letter-spacing:2px;border-left:5px solid #ff8a2a;padding-left:10px}
@media(max-width:760px) and (min-height:521px){.mode small{display:none}.mode b{min-width:0}.nb{font-size:14px;padding:7px 8px}}

.cards{display:grid;grid-template-columns:repeat(5,1fr);gap:6px;margin-bottom:10px}
.rcard{display:flex;flex-direction:column;align-items:center;justify-content:center;gap:6px;aspect-ratio:3/4;padding:6px 2px;border:0;border-bottom:4px solid transparent;border-radius:0;background:linear-gradient(180deg,rgba(255,255,255,.14),rgba(255,255,255,.04));color:#fff;transition:transform .12s,background .12s}
.rcard i{font-style:normal;font-size:clamp(24px,4vw,38px)}
.rcard b{font-size:clamp(11px,1.6vw,15px);font-style:italic;text-transform:uppercase;letter-spacing:1px;text-align:center;line-height:1.1}
.rcard:hover{transform:translateY(-3px)}
.rcard.on{background:linear-gradient(180deg,#ffffff,#bcd9ff);color:#0a1840;border-bottom-color:#ff8a2a;transform:translateY(-3px)}

/* ===== Menu principal comme sur la capture : liste verticale à gauche, bannière du joueur en bas à gauche ===== */
@media(min-width:761px),(max-width:760px) and (max-height:520px){
  #topbar{position:absolute;inset:0;padding:0;display:block;background:none;pointer-events:none}
  #topbar>*{pointer-events:auto}
  #nav{position:absolute;left:max(2.4vw,env(safe-area-inset-left));top:30%;flex-direction:column;flex-wrap:nowrap;justify-content:flex-start;gap:5px;width:min(25vw,280px)}
  .nb{display:block;width:100%;text-align:left;padding:11px 42px 11px 18px;font-size:clamp(15px,2vw,22px);font-style:normal;font-weight:700;letter-spacing:2px;text-transform:uppercase;color:#fff;
    background:linear-gradient(90deg,rgba(0,0,0,.85),rgba(0,0,0,.62));clip-path:polygon(0 0,100% 0,calc(100% - 16px) 100%,0 100%);transform:none}
  .nb:hover{background:linear-gradient(90deg,#c4601a,#ff9a3c);color:#fff;transform:none}
  .nb.on{background:linear-gradient(90deg,#1f6fe0,#9cc8ff);color:#fff;text-shadow:none;box-shadow:none}
  .nb.on::after{position:static;display:block;height:auto;transform:none;background:none;left:auto;right:auto;content:""}
  .nb.on[data-go="lobby"]::after{content:"SUR LE TERRAIN";font-size:11px;letter-spacing:2px;opacity:.9;margin-top:1px}
  #prof{position:absolute;left:max(2.4vw,env(safe-area-inset-left));bottom:max(3vh,env(safe-area-inset-bottom));min-width:250px;padding:8px 34px 8px 14px;border-left:0;clip-path:polygon(0 0,100% 0,calc(100% - 16px) 100%,0 100%)}
  #wallet{position:absolute;right:max(14px,env(safe-area-inset-right));top:max(10px,env(safe-area-inset-top))}
  #scr{top:56px}
  .rpanel{width:min(46vw,540px)}
  .cpanel{left:auto;right:12px;transform:none;width:min(62vw,760px)}
  .cpanel.wide{width:min(66vw,920px)}
  #lobinfo{left:auto;right:max(14px,env(safe-area-inset-right));bottom:auto;top:max(54px,env(safe-area-inset-top));align-items:flex-end;border-left:0;padding-left:0;font-size:18px;display:none}
}

/* accueil façon RL : seulement la liste + la voiture ; le panneau s'ouvre en choisissant JOUER */
#ui.home #scr,#ui.home #lobinfo{display:none}
#back{position:absolute;left:max(2.4vw,env(safe-area-inset-left));bottom:calc(max(3vh,env(safe-area-inset-bottom)) + 92px);pointer-events:auto;padding:9px 30px 9px 16px;border:0;font-weight:700;font-size:15px;letter-spacing:2px;text-transform:uppercase;
 background:linear-gradient(90deg,rgba(0,0,0,.85),rgba(0,0,0,.6));clip-path:polygon(0 0,100% 0,calc(100% - 14px) 100%,0 100%)}
#back:hover{background:linear-gradient(90deg,#c4601a,#ff9a3c)}
#ui .rcard.on,#ui .mode.on,#ui .opt.on{color:#0a1840}
#ui .mode.on b,#ui .mode.on small,#ui .rcard.on b,#ui .rcard.on i{color:#0a1840;text-shadow:none}
.cards{grid-template-columns:repeat(5,minmax(0,1fr))}
.rcard b{font-size:clamp(9px,1.15vw,14px);letter-spacing:.5px}
.rcard{min-width:0;overflow:hidden}

/* ===== accueil identique à la capture ===== */
#party,#music,#news{display:none}
#ui.home #party,#ui.home #music{display:flex}
#ui.home #news{display:block}
#party{position:absolute;left:calc(max(2.4vw,env(safe-area-inset-left)) + 262px);bottom:calc(max(3vh,env(safe-area-inset-bottom)) + 2px);align-items:center;pointer-events:auto;border:0;background:transparent;font-weight:700;font-size:13px;letter-spacing:1.5px;text-shadow:0 1px 6px #000}
#party:hover{color:#ffb02e}
#music{position:absolute;right:max(14px,env(safe-area-inset-right));bottom:max(3vh,env(safe-area-inset-bottom));align-items:center;gap:10px;padding:8px 22px 8px 8px;background:rgba(0,0,0,.8);clip-path:polygon(14px 0,100% 0,100% 100%,0 100%);min-width:240px}
#music{pointer-events:auto;cursor:pointer}
#music i{width:46px;height:46px;background:linear-gradient(135deg,#2f8bff,#ff8a2a);flex:none}
#music b{display:block;font-size:16px;font-style:italic;text-transform:uppercase;letter-spacing:1px}
#music small{display:block;font-size:11px;opacity:.7;letter-spacing:1px}
#ui #news{position:absolute;right:0;top:50%;transform:translateY(-50%);writing-mode:vertical-rl;padding:26px 11px;border:0;background:linear-gradient(90deg,#fff,#cfe0ff);color:#0a1840;font-weight:800;font-size:20px;letter-spacing:4px;pointer-events:auto;box-shadow:0 0 24px rgba(0,0,0,.5)}
#lvl{position:absolute;left:3px;top:50%;transform:translateY(-50%);width:46px;height:46px;border-radius:50%;background:#0a1124;border:3px solid #ff8a2a;display:flex;align-items:center;justify-content:center;font-size:17px;font-weight:800;margin:0;text-shadow:none}
@media(min-width:761px),(max-width:760px) and (max-height:520px){#prof{padding-left:58px}}
@media(max-width:760px) and (min-height:521px){#prof{position:relative;padding-left:40px}}

#chw{display:none}
#ui.home #chw{display:block;position:absolute;right:max(14px,env(safe-area-inset-right));top:calc(max(10px,env(safe-area-inset-top)) + 54px);width:min(250px,24vw);padding:10px 14px;background:rgba(0,0,0,.72);clip-path:polygon(0 0,100% 0,100% calc(100% - 14px),calc(100% - 14px) 100%,0 100%);pointer-events:auto;cursor:pointer}
#chw>b{display:block;font-size:13px;letter-spacing:3px;font-style:italic;margin-bottom:6px;color:#ffb02e}
#chw .cw{margin:6px 0;font-size:12px;line-height:1.25}
#chw .cw small{opacity:.7;font-size:10px}
.popb{font-style:normal;font-size:11px;opacity:.85;white-space:nowrap;margin-left:auto;padding-left:6px}
.qmenu{display:flex;gap:6px;justify-content:center;flex-wrap:wrap;margin:10px 0 4px}
.qmenu .go{width:auto;margin:0;padding:8px 12px;font-size:13px;letter-spacing:1px}
.newsw{display:flex;gap:12px;margin:8px 0 4px;min-height:170px}
.newsl{display:flex;flex-direction:column;gap:6px;width:36%;flex:none}
.nitem{padding:10px 12px;border:0;text-align:left;font-weight:700;font-size:14px;background:rgba(255,255,255,.08);clip-path:polygon(0 0,100% 0,calc(100% - 10px) 100%,0 100%)}
.nitem.on{background:linear-gradient(90deg,#fff,#bcd9ff);color:#0a1840!important}
.newsd{flex:1;padding:4px 6px}
.newsd h3{margin:0 0 8px;font-size:18px;font-style:italic;text-transform:uppercase;letter-spacing:1px;opacity:1}
.newsd p{font-size:14px;line-height:1.5;opacity:.85}
@media(max-width:760px) and (min-height:521px){#ui.home #chw{display:none}}
#friends,#leave{display:none}
#ui.home #friends{display:flex;position:absolute;right:max(14px,env(safe-area-inset-right));bottom:calc(max(3vh,env(safe-area-inset-bottom)) + 66px);align-items:center;padding:9px 22px 9px 26px;border:0;background:rgba(0,0,0,.8);clip-path:polygon(14px 0,100% 0,100% 100%,0 100%);font-weight:700;font-size:14px;letter-spacing:1.5px;pointer-events:auto}
#ui.home #friends:hover{background:linear-gradient(90deg,#c4601a,#ff9a3c)}
#ui.home.inparty #leave{display:block;position:absolute;left:50%;transform:translateX(-50%);bottom:max(3vh,env(safe-area-inset-bottom));pointer-events:auto;padding:9px 30px;border:0;font-weight:700;font-size:15px;letter-spacing:2px;background:linear-gradient(90deg,rgba(0,0,0,.85),rgba(0,0,0,.6));clip-path:polygon(14px 0,100% 0,calc(100% - 14px) 100%,0 100%)}
#ui.home.inparty #leave:hover{background:linear-gradient(90deg,#a02020,#ff5a4a)}
.opt[disabled]{opacity:.4;pointer-events:none}

/* ===== MENU HOMOGÈNE : un seul langage visuel =====
   - grandes tuiles : trapèze oblique (pointe vers l'intérieur de l'écran), fond noir dégradé
   - petits contrôles : parallélogramme
   - panneaux : angles droits, liseré bleu en haut
   - état actif : dégradé blanc/bleu clair + liseré orange
   - police unique (Rajdhani), titres en capitales italiques */
:root{--sk:16px;--sc:10px;--blue:#2f8bff;--orange:#ff8a2a;--ink:#0a1840;
 --tileL:linear-gradient(90deg,rgba(0,0,0,.85),rgba(0,0,0,.62));
 --tileR:linear-gradient(270deg,rgba(0,0,0,.85),rgba(0,0,0,.62));
 --shL:polygon(0 0,100% 0,calc(100% - var(--sk)) 100%,0 100%);
 --shR:polygon(var(--sk) 0,100% 0,100% 100%,0 100%);
 --shC:polygon(var(--sc) 0,100% 0,calc(100% - var(--sc)) 100%,0 100%);
 --pnl:linear-gradient(180deg,rgba(8,16,44,.88),rgba(4,8,24,.94));
 --on:linear-gradient(180deg,#ffffff,#bcd9ff);
 --font:"Rajdhani","Saira Condensed","Segoe UI",system-ui,sans-serif}
#ui,#ui button,#ui input,#queue,#queue button,#modal,#modal button,#toast,.panel,.go,.txt{font-family:var(--font)}
#ui,#queue,#modal{color-scheme:dark}
#ui input[type=range]{accent-color:var(--orange)}

/* panneaux : tous identiques */
.rpanel,.cpanel,.panel,#qbox,#mbox{border-radius:0;border:0;border-top:3px solid var(--blue);background:var(--pnl);box-shadow:0 0 60px rgba(0,0,0,.7)}
.cpanel h2,.phead h2{margin:0 0 4px;font-size:clamp(30px,5vw,44px);font-style:italic;font-weight:900;letter-spacing:3px;text-transform:uppercase}
.cpanel .sub,.phead .sub{text-transform:uppercase;letter-spacing:2px;font-size:12px}
h3{text-transform:uppercase;font-style:italic;letter-spacing:2px}
.pfoot{background:rgba(0,0,0,.35)}

/* onglets = petits contrôles obliques */
.tab{border-radius:0;clip-path:var(--shC);padding:9px 20px 9px 16px;background:rgba(255,255,255,.08);font-style:italic;font-weight:700;font-size:13px;letter-spacing:1px;text-transform:uppercase;color:#fff}
.tab.on{background:var(--on);color:var(--ink);box-shadow:inset 0 -3px 0 var(--orange)}
#ui .tab.on,#ui .chip2.on,#ui #news{color:var(--ink)}
.bar{border-radius:0;background:rgba(255,255,255,.14)}
.bar i{background:linear-gradient(90deg,var(--orange),#ffc15a)}
#chw .bar{height:5px;margin:4px 0 3px}
.tabs{gap:3px;border-bottom:1px solid rgba(255,255,255,.1)}
.opt,.chip2{clip-path:var(--shC)}
.chip2{border:0;border-radius:0;background:rgba(255,255,255,.1);padding:6px 16px;font-style:italic;text-transform:uppercase;letter-spacing:1px}
.chip2.on{background:var(--on);color:var(--ink)}
.go,.go.alt{clip-path:polygon(var(--sk) 0,100% 0,calc(100% - var(--sk)) 100%,0 100%)}

/* cartes et blocs : angles droits, même fond, même liseré */
.card,.pcard,.crate,.reveal,.rank,.stats div,.bund,.chal,.bm,.bt,.txt,.card .cv,div.cv{border-radius:0}
.card,.pcard,.stats div,.bm{background:linear-gradient(180deg,rgba(255,255,255,.10),rgba(255,255,255,.04))}
.bund,.chal{border:0;border-left:5px solid rgba(255,255,255,.25);background:linear-gradient(90deg,rgba(255,255,255,.09),rgba(255,255,255,.02));clip-path:var(--shL);padding-right:30px}
.card.eq{box-shadow:0 0 0 2px #7dffb0 inset}
.card.sel{box-shadow:0 0 0 2px var(--orange) inset}
.txt{border:0;border-bottom:2px solid var(--blue);background:rgba(0,0,0,.5);font-family:var(--font);font-size:16px}
.nitem{clip-path:var(--shL)}
.mode{clip-path:var(--shL)}

/* barre du haut : monnaie + récompense = même hauteur, même forme */
#wallet{align-items:stretch;gap:6px}
#cred,#daily{display:flex;align-items:center;height:42px;border:0;border-radius:0;clip-path:var(--shR);padding:0 24px 0 28px;font-size:17px;font-weight:800;font-style:italic;letter-spacing:1px;text-transform:uppercase}
#cred{background:var(--tileR);color:#fff}
#daily{background:rgba(255,255,255,.14);color:#fff;box-shadow:none}
#daily.pulse{background:linear-gradient(90deg,#ff7a1a,#ffb02e);color:#1a0d00;box-shadow:none;animation:pl2 1.2s infinite}
@keyframes pl2{50%{filter:brightness(1.3)}}

/* widgets de l'accueil : même fond, même pente, même police */
@media(min-width:761px),(max-width:760px) and (max-height:520px){
  #prof,#back,#party,#leave,#nav .nb{clip-path:var(--shL)}
  #back{bottom:auto;top:calc(30% - 54px)} /* au-dessus de la liste : ne chevauche plus QUITTER */
  #party,#back{padding:11px 36px 11px 18px;border:0;background:var(--tileL);color:#fff;font-size:15px;font-weight:700;letter-spacing:2px;text-transform:uppercase;text-shadow:none}
  #ui.home #party{display:flex;align-items:center;bottom:max(3vh,env(safe-area-inset-bottom));height:44px}
  #party:hover,#back:hover{background:linear-gradient(90deg,#c4601a,#ff9a3c);color:#fff}
  #ui.home.inparty #leave{clip-path:var(--shL);padding:11px 36px 11px 18px;background:var(--tileL);font-size:15px;letter-spacing:2px;text-transform:uppercase}
}
/* petit écran : un menu ouvert ne prend plus tout l'écran, la navigation reste visible au-dessus */
@media(max-width:760px) and (min-height:521px){
  .cpanel,.cpanel.wide{left:max(10px,env(safe-area-inset-left));right:max(10px,env(safe-area-inset-right));top:auto;bottom:max(8px,env(safe-area-inset-bottom));transform:none;width:auto;height:56%;max-height:none}
  #back{bottom:calc(max(8px,env(safe-area-inset-bottom)) + (100% - 56px)*.56 + 8px)}
}
/* téléphone en paysage : liste du menu compactée pour ne pas passer sous la bannière du joueur */
@media(max-height:520px){
  #back{top:10px;padding:8px 30px 8px 16px;font-size:13px}
  #nav{top:56px;gap:3px}
  .nb{padding:7px 36px 7px 16px;font-size:14px}
  #nav .nb.on[data-go="lobby"]::after{font-size:9px}
  #prof{min-width:210px;padding:5px 30px 5px 58px}
  #pname{font-size:16px}
  #lvl{width:38px;height:38px;font-size:15px}
}
@media(max-width:760px) and (min-height:521px){
  #prof{padding-left:62px} /* le rond du niveau ne recouvre plus le pseudo */
  #ui.home #party{left:12px;bottom:calc(max(3vh,env(safe-area-inset-bottom)) + 124px);height:42px;padding:10px 30px 10px 16px;background:var(--tileL);clip-path:var(--shL);color:#fff;font-size:14px;letter-spacing:2px;text-transform:uppercase;text-shadow:none} /* ne passe plus derrière « Fanfare » */
}
#ui.home #chw,#ui.home #friends,#music{clip-path:var(--shR);background:var(--tileR);border:0}
#ui.home #friends,#music{font-family:var(--font);text-transform:uppercase}
#ui.home #friends{padding:11px 22px 11px 34px;font-size:15px;font-weight:700;letter-spacing:2px}
#music{padding:8px 22px 8px 30px}
#music i{clip-path:var(--shC)}
#chw{padding:12px 18px 12px 30px}
#ui.home #chw{padding:12px 18px 12px 30px;width:min(270px,26vw)}
#chw>b{font-size:15px;letter-spacing:3px;text-transform:uppercase;color:var(--orange);margin-bottom:8px}
#chw .cw{font-size:14px;font-weight:700;margin:8px 0;line-height:1.2}
#chw .cw small{display:block;font-size:11px;font-weight:600;letter-spacing:1px;opacity:.7;margin-top:2px}
#ui #news{clip-path:polygon(0 var(--sk),100% 0,100% 100%,0 100%);background:var(--on);font-family:var(--font);font-weight:700;padding:34px 12px 26px}
#ui #news:hover{background:linear-gradient(180deg,#ffd9b0,#ff9a3c)}
#toast{border:0;border-left:4px solid var(--orange);border-radius:0;font-family:var(--font);font-size:16px;letter-spacing:1px;background:rgba(4,8,24,.96)}

/* ===== MENU OUVERT = PLEIN ÉCRAN (le bouton RETOUR ramène à l'accueil) ===== */
#topbar{z-index:2}#scr{z-index:1}#back{z-index:3}
#ui:not(.home) #nav,#ui:not(.home) #prof,#ui:not(.home) #lobinfo{display:none}
#ui:not(.home) #topbar{background:none;padding:0;pointer-events:none}
#ui:not(.home) #topbar>*{pointer-events:auto}
#ui:not(.home) #wallet{position:absolute;right:max(10px,env(safe-area-inset-right));top:max(8px,env(safe-area-inset-top));z-index:3}
#ui:not(.home) #back{display:flex;align-items:center;left:max(10px,env(safe-area-inset-left));top:max(8px,env(safe-area-inset-top));bottom:auto;height:42px;padding:0 36px 0 18px;font-size:15px}
#ui:not(.home) #scr{top:0;left:0;right:0;bottom:0}
#ui:not(.home) .rpanel,#ui:not(.home) .cpanel,#ui:not(.home) .cpanel.wide{position:absolute;left:0;right:0;top:0;bottom:0;width:auto;height:auto;max-height:none;transform:none;border:0;
  padding-top:calc(max(8px,env(safe-area-inset-top)) + 56px);background:linear-gradient(180deg,rgba(4,8,24,.9),rgba(4,8,24,.96))}
#ui:not(.home) .rpanel{padding-left:max(0px,env(safe-area-inset-left));padding-right:max(0px,env(safe-area-inset-right))}
#ui:not(.home) .cpanel{padding-left:max(18px,env(safe-area-inset-left));padding-right:max(18px,env(safe-area-inset-right))}
/* sur grand écran, le contenu reste centré et lisible */
#ui:not(.home) .cpanel>*,#ui:not(.home) .phead,#ui:not(.home) .tabs,#ui:not(.home) .pbody,#ui:not(.home) .pfoot>*{max-width:1000px;width:100%;margin-left:auto;margin-right:auto}
#ui:not(.home) .tabs{width:100%}
@media(max-width:760px) and (min-height:521px){
  #ui:not(.home) #cred,#ui:not(.home) #daily{height:36px;font-size:13px;padding:0 14px 0 22px}
  #ui:not(.home) #back{height:36px;padding:0 28px 0 12px;font-size:13px}
  #ui:not(.home) .rpanel,#ui:not(.home) .cpanel,#ui:not(.home) .cpanel.wide{padding-top:calc(max(8px,env(safe-area-inset-top)) + 50px)}
}

body.gp-on #touch{display:none!important}
/* ===== MANETTE : surbrillance de l'élément sélectionné ===== */
.gpf{box-shadow:inset 0 0 0 3px #ffb02e,inset 0 0 24px rgba(255,176,46,.55)!important;filter:brightness(1.25)}
.cards{grid-template-columns:repeat(6,minmax(0,1fr))!important}

/* ===== ACCUEIL RÉORGANISÉ (paysage) =====
   Colonne gauche  : bannière du joueur (haut) → liste du menu (qui se compresse selon la hauteur de l'écran)
   Bas, au centre  : CRÉER UNE ÉQUIPE (et QUITTER LE GROUPE juste au-dessus)
   Colonne droite  : RÉCOMPENSE + argent → DÉFIS → … → AMIS → MUSIQUE (même largeur, même bord, rien ne se chevauche)
   Onglet NEWS     : sur le bord droit, la colonne droite lui laisse la place */
@media(min-width:761px),(max-width:760px) and (max-height:520px){
  #ui.home{
    --mx:max(2.4vw,env(safe-area-inset-left));
    --my:max(10px,env(safe-area-inset-top));
    --by:max(3vh,env(safe-area-inset-bottom));
    --newsw:48px;
    --rx:calc(max(8px,env(safe-area-inset-right)) + var(--newsw) + 8px);
    --navw:clamp(190px,25vw,280px);
    --colw:clamp(200px,26vw,270px);
    --ph:clamp(46px,8vh,60px);
    --wh:clamp(32px,6vh,42px);
    --mh:clamp(42px,11vh,62px);
    --fh:clamp(32px,8vh,42px);
    --ph2:clamp(36px,8vh,44px);
  }
  /* gauche : joueur en haut, liste dessous */
  #ui.home #prof{top:var(--my);bottom:auto;left:var(--mx);width:var(--navw);min-width:0;height:var(--ph);box-sizing:border-box;padding-top:0;padding-bottom:0}
  #ui.home #nav{align-self:stretch;top:calc(var(--my) + var(--ph) + 8px);bottom:var(--by);left:var(--mx);width:var(--navw);gap:3px;display:flex;flex-direction:column;justify-content:flex-start}
  #ui.home #nav .nb{flex:0 1 52px;min-height:0;display:flex;flex-direction:column;justify-content:center;box-sizing:border-box;padding-top:0;padding-bottom:0;font-size:clamp(12px,3.6vh,22px);line-height:1.1}
  #ui.home #nav .nb.on[data-go="lobby"]::after{font-size:clamp(8px,2.2vh,11px);margin-top:1px;line-height:1}
  /* bas : équipe au centre-gauche, « quitter le groupe » juste au-dessus */
  #ui.home #party{left:calc(var(--mx) + var(--navw) + 12px);bottom:var(--by);height:var(--ph2);padding-top:0;padding-bottom:0;font-size:clamp(12px,3vh,15px)}
  #ui.home.inparty #leave{left:calc(var(--mx) + var(--navw) + 12px);transform:none;bottom:calc(var(--by) + var(--ph2) + 6px);height:var(--ph2);padding-top:0;padding-bottom:0;font-size:clamp(12px,3vh,15px)}
  /* droite : une seule colonne alignée */
  #ui.home #wallet{right:var(--rx);top:var(--my)}
  #ui.home #cred,#ui.home #daily{height:var(--wh);font-size:clamp(13px,3.2vh,17px);padding:0 22px 0 26px}
  #ui.home #chw{right:var(--rx);top:calc(var(--my) + var(--wh) + 8px);width:var(--colw);box-sizing:border-box;padding:clamp(6px,1.6vh,12px) 16px clamp(6px,1.6vh,12px) 28px}
  #ui.home #chw>b{font-size:clamp(12px,3vh,15px);margin-bottom:clamp(2px,.8vh,8px)}
  #ui.home #chw .cw{display:grid;grid-template-columns:minmax(0,1fr) auto;column-gap:8px;align-items:baseline;margin:clamp(3px,1vh,8px) 0;font-size:clamp(11px,2.7vh,14px);line-height:1.15}
  #ui.home #chw .cw span{min-width:0}
  #ui.home #chw .cw .bar{grid-column:1/-1;grid-row:2;margin:3px 0 0}
  #ui.home #chw .cw small{display:block;margin:0;font-size:clamp(10px,2.4vh,11px);white-space:nowrap}
  #ui.home #friends{right:var(--rx);bottom:calc(var(--by) + var(--mh) + 6px);width:var(--colw);height:var(--fh);box-sizing:border-box;padding-top:0;padding-bottom:0;font-size:clamp(12px,3vh,15px);white-space:nowrap}
  #ui.home #music{right:var(--rx);bottom:var(--by);width:var(--colw);min-width:0;height:var(--mh);box-sizing:border-box;padding-top:5px;padding-bottom:5px}
  #ui.home #music i{width:calc(var(--mh) - 10px);height:calc(var(--mh) - 10px)}
  #ui.home #music b{font-size:clamp(13px,3.2vh,16px)}
  #ui.home #music small{font-size:clamp(9px,2.3vh,11px)}
  /* onglet NEWS sur le bord, jamais sous une autre case */
  #ui #news{font-size:clamp(14px,3.6vh,20px);padding:clamp(20px,4vh,34px) clamp(8px,1.6vh,12px) clamp(16px,3vh,26px)}
}
@media(max-height:520px){
  #ui.home{--newsw:38px}
  #ui.home #prof{min-width:0;padding:0 30px 0 58px}
}
/* portrait : l'onglet NEWS descend sous la liste et la ligne récompense pour ne plus les recouvrir */
@media(max-width:760px) and (min-height:521px){
  #ui #news{top:calc(50% + 85px)}
}

/* ===== MENUS PLEIN ÉCRAN EN PAYSAGE (téléphone couché) =====
   Une seule barre en haut : RETOUR à gauche, le TITRE à côté, RÉCOMPENSE + argent à droite.
   Tout le reste de la hauteur sert au contenu (liste, cartes, réglages, éditeur…). */
@media(max-height:520px){
  #ui:not(.home){--hb:34px;--ht:max(5px,env(safe-area-inset-top));--hx:max(8px,env(safe-area-inset-left));--hbar:calc(var(--ht) + var(--hb) + 6px)}
  #ui:not(.home) #back{left:var(--hx);top:var(--ht);height:var(--hb);padding:0 26px 0 14px;font-size:13px;letter-spacing:1.5px}
  #ui:not(.home) #wallet{top:var(--ht);right:max(8px,env(safe-area-inset-right));gap:4px}
  #ui:not(.home) #cred,#ui:not(.home) #daily{height:var(--hb);font-size:13px;padding:0 16px 0 22px}
  #ui:not(.home) .rpanel,#ui:not(.home) .cpanel,#ui:not(.home) .cpanel.wide{padding-top:var(--hbar)}
  #ui:not(.home) .cmk{padding-top:var(--hbar)!important;padding-bottom:6px!important}
  /* titre dans la barre du haut */
  #ui:not(.home) .phead>h2,#ui:not(.home) .cpanel>h2:first-child{position:absolute;top:var(--ht);left:calc(var(--hx) + 150px);width:auto;max-width:calc(100% - 150px - 290px - var(--hx));margin:0;height:var(--hb);line-height:var(--hb);font-size:22px;letter-spacing:2px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;z-index:2}
  #ui:not(.home) .phead{padding:0 16px 2px}
  #ui:not(.home) .phead .sub,#ui:not(.home) .cpanel>.sub{font-size:10px;letter-spacing:1.5px;margin:0 0 4px}
  #ui:not(.home) .pfoot{padding:6px 14px}
  #ui:not(.home) .pbody{padding:6px 14px}
  #ui:not(.home) .tabs{padding:4px 10px 0}
  #ui:not(.home) .tab{padding:6px 16px 6px 12px;font-size:12px}
}

/* téléphone couché étroit (≈ 570–700 px) : colonnes plus fines pour que « Créer une équipe » et la musique ne se touchent pas */
@media(max-width:700px) and (max-height:520px){
  #ui.home{--navw:150px;--colw:max(150px,24vw)}
  #ui.home #nav .nb{padding-left:12px;padding-right:22px;letter-spacing:1px}
  #ui.home #party,#ui.home.inparty #leave{padding:0 22px 0 12px;letter-spacing:1px;font-size:clamp(11px,3vh,13px)}
  #ui.home #friends{padding:0 12px 0 22px;letter-spacing:1px;font-size:clamp(11px,3vh,13px)}
  #ui.home #music{padding-left:22px;padding-right:10px;gap:6px;overflow:hidden}
  #ui.home #music b,#ui.home #music small{white-space:nowrap;overflow:hidden;text-overflow:ellipsis;max-width:100%}
  #ui.home #chw{padding-left:22px}
  #ui.home #prof{padding-left:48px;padding-right:18px}
  #ui.home #lvl{width:32px;height:32px;font-size:14px}
  #ui.home #pname{font-size:14px;letter-spacing:0;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
  #ui.home #friends{letter-spacing:.5px;font-size:11px;padding-right:6px}
}
/* très petit écran couché (≈ 320 px de haut) : les barres de progression des défis disparaissent, les compteurs restent */
@media(max-height:340px){
  #ui.home #chw .cw .bar{display:none}
  #ui.home #chw .cw{margin:3px 0}
}

/* les commandes tactiles ne sélectionnent plus leur texte et n'ouvrent pas la loupe/menu iOS au double toucher */
.btn,#touch,#hud{-webkit-user-select:none;user-select:none;-webkit-touch-callout:none;-webkit-tap-highlight-color:transparent}

/* fenêtre NEWS : elle était plus large que sa boîte et sortait de l'écran ; elle prend maintenant toute la place disponible et la liste défile */
#mbox:has(.newsw){width:min(94vw,780px);max-height:96vh;padding:0;overflow:hidden;display:flex}
#mbox:has(.newsw)>.panel{width:100%!important;max-height:96vh;margin:0;box-sizing:border-box;overflow:auto}
.newsw{min-width:0}
.newsl{max-height:62vh;overflow:auto;min-width:0}
.newsd{min-width:0;overflow-wrap:anywhere}
@media(max-height:520px){
  #mbox:has(.newsw)>.panel h2{font-size:22px;margin:0 0 4px}
  #mbox:has(.newsw)>.panel{padding:10px 14px}
  .newsw{min-height:0;margin:4px 0}
  .newsl{max-height:calc(96vh - 110px);gap:4px}
  .nitem{padding:7px 12px;font-size:13px}
  .newsd h3{font-size:16px;margin:0 0 4px}
  .newsd p{font-size:13px;line-height:1.35}
}

/* ===== TRANSITIONS =====
   - fenêtres (pause, fin de match, recherche, modales) : fondu + léger zoom, à l'ouverture ET à la fermeture
   - menus : fondu à l'entrée/sortie, éléments de l'accueil qui glissent un à un, écran plein écran qui monte
   - rideau noir entre le menu et le match
   - survols, appuis et barres de progression adoucis */
:root{--ease:cubic-bezier(.22,.8,.24,1)}
#pause,#end,#queue,#modal{display:flex;opacity:0;visibility:hidden;pointer-events:none;transition:opacity .28s ease,visibility 0s linear .28s}
#pause.show,#end.show,#queue.show,#modal.show{opacity:1;visibility:visible;pointer-events:auto;transition:opacity .28s ease,visibility 0s}
#curtain{position:fixed;inset:0;background:#02040c;opacity:0;pointer-events:none;z-index:15}
.tab,.opt,.chip2,.nitem,.card,.pcard,.crate,.swb{transition:transform .16s var(--ease),filter .16s ease,box-shadow .2s ease,opacity .2s ease}
.btn{transition:background .12s ease}
.bar i,#xpbar{transition:width .6s var(--ease)}
@media(hover:hover){
  .opt:hover,.chip2:hover,.tab:hover,.nitem:hover{filter:brightness(1.18)}
  .card:not(.sel):hover,.crate:hover{transform:translateY(-2px)}
}
.opt:active,.chip2:active,.tab:active,.nitem:active,.card:active,.go:active,.rcard:active,.nb:active{transform:scale(.97)}
.mode:active{transform:translateX(4px) scale(.98)}
@media(prefers-reduced-motion:no-preference){
  #pause>.panel,#end>.panel,#qbox,#mbox{transform:translateY(18px) scale(.96);transition:transform .34s var(--ease)}
  #pause.show>.panel,#end.show>.panel,#queue.show #qbox,#modal.show #mbox{transform:none}
  @keyframes uiIn{from{opacity:0}}
  @keyframes uiOut{to{opacity:0}}
  @keyframes fromL{from{opacity:0;translate:-36px 0}}
  @keyframes fromR{from{opacity:0;translate:36px 0}}
  @keyframes fromB{from{opacity:0;translate:0 24px}}
  @keyframes scrIn{from{opacity:0;translate:0 28px}}
  @keyframes fadeUp{from{opacity:0;translate:0 10px}}
  #ui.enter{animation:uiIn .4s ease backwards}
  #ui.enter.home #prof{animation:fromL .5s var(--ease) backwards}
  #ui.enter.home #nav .nb{animation:fromL .45s var(--ease) backwards}
  #ui.enter.home #nav .nb:nth-child(1){animation-delay:.04s}
  #ui.enter.home #nav .nb:nth-child(2){animation-delay:.08s}
  #ui.enter.home #nav .nb:nth-child(3){animation-delay:.12s}
  #ui.enter.home #nav .nb:nth-child(4){animation-delay:.16s}
  #ui.enter.home #nav .nb:nth-child(5){animation-delay:.2s}
  #ui.enter.home #nav .nb:nth-child(6){animation-delay:.24s}
  #ui.enter.home #nav .nb:nth-child(7){animation-delay:.28s}
  #ui.enter.home #nav .nb:nth-child(8){animation-delay:.32s}
  #ui.enter.home #nav .nb:nth-child(9){animation-delay:.36s}
  #ui.enter.home #wallet,#ui.enter.home #news{animation:fromR .5s var(--ease) backwards}
  #ui.enter.home #chw{animation:fromR .5s var(--ease) .1s backwards}
  #ui.enter.home #friends{animation:fromR .5s var(--ease) .18s backwards}
  #ui.enter.home #music{animation:fromR .5s var(--ease) .26s backwards}
  #ui.enter.home #party,#ui.enter.home #leave{animation:fromB .5s var(--ease) .3s backwards}
  #scr.anim>*{animation:scrIn .42s var(--ease) backwards}
  #scr.tabfade .pbody{animation:fadeUp .3s var(--ease) backwards}
  #ui.out{animation:uiOut .3s ease forwards}
}
#ui.out,#ui.out *{pointer-events:none!important}

/* ===== INTRO DU PREMIER LANCEMENT : bandes de cinéma, titre, HUD masqué ===== */
body.intro #hud,body.intro #touch,body.intro #msg,body.intro #rtag,body.intro #sb{display:none!important}
#introFx{position:fixed;inset:0;z-index:16;pointer-events:none;display:none;font-family:var(--font)}
#introFx.show{display:block}
#introFx .bar{position:absolute;left:0;right:0;height:11vh;background:#000;transition:transform .9s var(--ease)}
#introFx .top{top:0;transform:translateY(-100%)}
#introFx .bot{bottom:0;transform:translateY(100%)}
#introFx.in .bar{transform:none}
#introTitle{position:absolute;left:0;right:0;top:34%;text-align:center;opacity:0;padding:0 4vw}
#introTitle b{display:block;font-size:clamp(30px,min(12vmin,9.5vw),130px);font-style:italic;font-weight:900;letter-spacing:4px;line-height:1.02;color:#fff;text-shadow:0 0 22px #ffb02e,0 0 64px #ff7a1a,0 5px 20px #000}
#introTitle b i{font-style:inherit;color:#ffb02e}
#introRing{position:absolute;left:50%;top:41%;width:14vmin;height:14vmin;margin:-7vmin 0 0 -7vmin;border-radius:50%;border:5px solid #fff;box-shadow:0 0 40px #ffb02e,inset 0 0 40px #ffb02e;opacity:0}
#introSkip{position:absolute;right:max(4vw,env(safe-area-inset-right));bottom:calc(11vh + 3vh);font-size:clamp(10px,2vmin,14px);font-weight:700;letter-spacing:3px;opacity:.7;text-shadow:0 1px 8px #000}
@keyframes introTitle{0%{opacity:0;translate:-40px 0}12%{opacity:1;translate:0 0}82%{opacity:1;translate:0 0}100%{opacity:0;translate:30px 0}}
@keyframes introBoom{0%{opacity:0;transform:scale(2.6);letter-spacing:44px;filter:blur(16px)}7%{opacity:1;transform:scale(.93);letter-spacing:4px;filter:blur(0)}13%{transform:scale(1.05)}20%{transform:scale(1)}84%{opacity:1;transform:scale(1)}100%{opacity:0;transform:scale(1.07)}}
@keyframes introRing{0%{opacity:.95;transform:scale(.1)}100%{opacity:0;transform:scale(11)}}
#introFx.boom #introTitle{animation:introBoom 5.2s ease-out both}
#introFx.boom #introRing{animation:introRing 1.1s cubic-bezier(.1,.7,.3,1) both}
</style>
</head>
<body>
<canvas id="c"></canvas>

<div id="hud" class="hide">
  <div id="score">
    <div class="tm tb"><span>BLEU</span><i id="sB">0</i></div>
    <div id="clockBox"><div id="clock">3:00</div><div id="ot"></div></div>
    <div class="tm to"><i id="sO">0</i><span>ORANGE</span></div>
  </div>
  <div id="grHud" class="hide"></div>
  <div id="boost"><div id="ring"><div id="bnum">33</div></div><div id="spd">0 km/h</div></div>
  <div id="feed"></div>
  <div id="pops"></div>
  <div id="sprint">SUPERSONIQUE</div>
  <div id="pw"></div>
  <div id="mv" class="hide">
    <div id="mvBar"><div id="mvS" class="demo"></div><button id="mvMin" type="button" title="Réduire / agrandir">–</button></div>
    <div id="mvBody">
    <div id="mvT"></div>
    <div id="mvD"></div>
    <ol id="mvL"></ol>
    <div id="mvG"></div>
    <div id="mvC"></div>
    <div id="mvB"><button data-mv="demo">▶ Revoir la démo (V)</button><button data-mv="reset">↻ Recommencer (R)</button><button data-mv="skip">Passer la démo (Entrée)</button><button data-mv="prev">◀ Précédent (B)</button><button data-mv="next">Suivant ▶ (N)</button></div>
    </div>
  </div>
</div>
<div id="vig"></div><div id="spdl"></div>
<div id="sb" class="ov"><div class="panel"><h2 id="sbT" style="text-align:center">TABLEAU DES SCORES</h2><div id="sbTable"></div></div></div>
<div id="msg"><div id="mbig"></div><div id="msub"></div></div>
<div id="flash"></div>
<div id="curtain"></div>
<div id="introFx"><div class="bar top"></div><div class="bar bot"></div><div id="introRing"></div><div id="introTitle"><b>ROCKET ARENA <i>3D</i></b></div><div id="introSkip">TOUCHE POUR PASSER ▶</div></div>
<div id="rtag" class="hide">▶ REPLAY<small>touche l'écran pour passer</small></div>

<div id="touch" class="hide">
  <div id="joyZone"></div>
  <div id="joyBase"><div id="joyKnob"></div></div>
  <div class="btn" id="bJump">SAUT</div>
  <div class="btn" id="bBoost">BOOST</div>
  <div class="btn" id="bDrift">DÉRAPE</div>
  <div class="btn" id="bCam">⌖</div>
  <div class="btn" id="bPause">❚❚</div>
  <div class="btn hide" id="bPower">POUVOIR</div>
</div>

<div id="ui">
  <div id="topbar">
    <div id="prof"><div id="pname"></div><div id="ptitle"></div><div id="lvl"></div><div class="xpb"><div id="xpbar"></div></div></div>
    <div id="nav"></div>
    <div id="wallet"><button id="daily">🎁 Récompense</button><div id="cred"></div></div>
  </div>
  <div id="scr"></div>
  <button id="back" class="hide">◀ RETOUR</button>
  <button id="party">⊕ CRÉER UNE ÉQUIPE</button>
  <button id="friends">👥 AMIS</button>
  <button id="leave">QUITTER LE GROUPE</button>
  <div id="music"><i></i><div><b id="mtitle"></b><small id="mlab">MUSIQUE DU MENU</small></div></div>
  <button id="news">NEWS</button>
  <div id="chw"></div>
</div>
<div id="queue"><div id="qbox"></div></div>
<div id="modal"><div id="mbox"></div></div>
<div id="toast"></div>
<div id="pause" class="ov">
  <div class="panel small"><h2>PAUSE</h2>
    <button id="resume" class="go">Reprendre</button>
    <button id="resetShot" class="go alt" style="display:none">↺ Réinitialiser le tir</button>
    <button id="quit" class="go alt">Quitter le match</button>
  </div>
</div>
<div id="end" class="ov">
  <div class="panel"><h2 id="endT"></h2><div class="sub" id="endS"></div><div id="endTable"></div><div id="endRew"></div>
    <button id="again" class="go">Rejouer</button>
    <button id="toMenu" class="go alt">Menu</button>
  </div>
</div>

<script type="module">
/* ---------- Firebase (Firestore) : classement en ligne ---------- */
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.14.1/firebase-app.js";
import { getFirestore, collection, doc, setDoc, getDoc, getDocs, query, orderBy, limit, where, getCountFromServer, serverTimestamp, runTransaction, deleteDoc } from "https://www.gstatic.com/firebasejs/10.14.1/firebase-firestore.js";
import { getAuth, onAuthStateChanged, signInWithPopup, signInWithRedirect, GoogleAuthProvider, createUserWithEmailAndPassword, signInWithEmailAndPassword, signOut, sendPasswordResetEmail, setPersistence, browserLocalPersistence } from "https://www.gstatic.com/firebasejs/10.14.1/firebase-auth.js";
const firebaseConfig = {
  apiKey: "AIzaSyDfFfxNgSLB_uHYbbu212t_wTjrZqGh5-U",
  authDomain: "rocketarena.firebaseapp.com",
  projectId: "rocketarena",
  storageBucket: "rocketarena.firebasestorage.app",
  messagingSenderId: "658432264155",
  appId: "1:658432264155:web:53dcb63725bdee02bd09fe"
};
try{
  const app = initializeApp(firebaseConfig);
  const db = getFirestore(app);
  let auth = null;
  try{ auth = getAuth(app); auth.languageCode = 'fr'; setPersistence(auth, browserLocalPersistence).catch(function(e){ console.warn('Persistance de session', e); }); }catch(e){ console.warn('Firebase Auth indisponible', e); }
  window.RA_FB = { db, collection, doc, setDoc, getDoc, getDocs, query, orderBy, limit, where, getCountFromServer, serverTimestamp, runTransaction, deleteDoc,
    auth, onAuthStateChanged, signInWithPopup, signInWithRedirect, GoogleAuthProvider, createUserWithEmailAndPassword, signInWithEmailAndPassword, signOut, sendPasswordResetEmail };
}catch(e){ window.RA_FB_ERR = e; console.warn('Firebase indisponible', e); }
window.dispatchEvent(new Event('ra-fb'));
</script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
<script>
(function(){
'use strict';
const T=THREE;

/* =====================================================================
   CONSTANTES
   ===================================================================== */
let HX=40,HZ=60,H=24,HY=12;const R=7;          // arène (demi-largeur, demi-longueur, hauteur, rayon des rampes)
let GW=10,GH=9,GD=12;                    // cage : demi-largeur, hauteur, profondeur
let BR=2.6,G=15.3,BMAX=141;const CR=0.9,STICK=24;
/* PHYSIQUE : réelle (1 unité = 42,6 uu de RL) ou héritée pour les modes à scripts (Mouvements / Entraînement) */
const PHR={g:15.3,jump:6.85,hold:34.2,flip:6.85,dodge:11.7,boost:23.3,stick:12,bmax:141};
const PHL={g:30,jump:11,hold:38,flip:10.5,dodge:15,boost:36,stick:24,bmax:82};
let PH=PHR;
const TEAM=[0x2f8bff,0xff8a2a],TCSS=['#4a9bff','#ffa045'],TNAME=['BLEU','ORANGE'];
const ACC=['#ffffff','#ffd400','#ff2d55','#32ff7e','#b967ff','#00e5ff'];
const BOTN=['Nova','Blaze','Vortex','Pixel','Turbo','Zephyr','Comet','Rogue','Drift','Apex','Echo','Flux'];
const clamp=(v,a,b)=>v<a?a:v>b?b:v;
const lerp=(a,b,t)=>a+(b-a)*t;
const rnd=Math.random;
const dampT=(r,dt)=>1-Math.exp(-r*dt);
const V3=(x,y,z)=>new T.Vector3(x||0,y||0,z||0);
const isTouch=(('ontouchstart' in window)||(navigator.maxTouchPoints>0))&&matchMedia('(pointer:coarse)').matches;
if(isTouch)document.body.classList.add('touch');
/* zoom désactivé : pincement (iOS/Android), double-tap (via touch-action), Ctrl+molette, Ctrl +/-/0 */
['gesturestart','gesturechange','gestureend'].forEach(t=>document.addEventListener(t,e=>e.preventDefault(),{passive:false}));
document.addEventListener('touchmove',e=>{if(e.touches.length>1)e.preventDefault();},{passive:false});
window.addEventListener('wheel',e=>{if(e.ctrlKey)e.preventDefault();},{passive:false});
/* zoom (2) : on annule dès le début du toucher sur les commandes de jeu et le terrain (rien d'autre à faire dessus : tout passe par les pointer events) */
document.addEventListener('touchstart',e=>{const t=e.target;if(t&&t.closest&&t.closest('#touch,#c'))e.preventDefault();},{passive:false});
/* zoom (3) filet de sécurité : si Safari zoomait quand même, on remet tout de suite l'échelle à 1 */
if(window.visualViewport){const mv=document.querySelector('meta[name="viewport"]'),base=mv?mv.content:'';let busy=false;
  const unzoom=()=>{if(busy||!mv||visualViewport.scale<=1.001)return;busy=true;mv.content=base.replace('initial-scale=1','initial-scale=1.0001');
    requestAnimationFrame(()=>{mv.content=base;window.scrollTo(0,0);setTimeout(()=>{busy=false;},120);});};
  visualViewport.addEventListener('resize',unzoom);visualViewport.addEventListener('scroll',unzoom);}
/* double-tap : Safari iPhone ignore parfois touch-action → on annule le 2e « touchend » rapproché sur les commandes de jeu, le terrain et les zones vides (les boutons des menus gardent leurs clics) */
{let lastEnd=0;document.addEventListener('touchend',e=>{const now=Date.now(),t=e.target,c=t&&t.closest?t:null;
  const inGame=!!(c&&c.closest('#touch,#c,#hud')),inter=!!(c&&c.closest('button,input,select,textarea,a,label,summary,[role="button"]'));
  if(now-lastEnd<400&&(inGame||!inter))e.preventDefault();lastEnd=now;},{passive:false});}
window.addEventListener('keydown',e=>{if((e.ctrlKey||e.metaKey)&&['+','-','=','_','0'].indexOf(e.key)>=0)e.preventDefault();});

const cfg={size:2,diff:1,dur:3,team:0,ballCam:1,autoThr:1,mode:'casual',arena:'std',autoPilot:false,skill:0,pack:0};
/* ---------- arènes (une par type de mode) ---------- */
const ARENAS={
  std:{HX:96,HZ:120,H:48,GW:21,GH:15,GD:21,goals:true,name:'Stade'},
  snow:{HX:96,HZ:120,H:48,GW:21,GH:15,GD:21,goals:true,name:'Patinoire'},
  hoops:{HX:69.5,HZ:83.9,H:42.7,GW:0,GH:0,GD:0,goals:false,name:'Dunk House'}, /* taille réelle du Hoops (≈ 2966 × 3581 × 1820 uu, 1 u = 42,67 uu comme le Stade) */
  drop:{HX:132.79,HZ:115,H:48,GW:0,GH:0,GD:0,goals:false,name:'Hexa-Arène'} /* hexagone régulier à l'échelle du Stade (même hauteur, même aire) : HZ = apothème (murs plats côté équipes), HX = rayon (sommets sur ±x) */
};
const CM={cur:null,pend:null,KEY:'rocketArena3D.maps.v1'}; /* carte personnalisée en cours */
const ARN={kind:'std',tex:null,wtex:null};
/* texture des murs choisie par le joueur (image), gardée dans le navigateur */
const WALL={KEY:'rocketArena3D.wallImg',tex:null,url:null,rep:1};
function wallApplyRep(){if(WALL.tex)WALL.tex.repeat.set(WALL.rep,WALL.rep);}
function wallLoad(){try{const j=JSON.parse(localStorage.getItem(WALL.KEY));if(j&&j.d){WALL.rep=+j.r||1;wallFromUrl(j.d,true,null);}}catch(e){}}
function wallSave(){try{localStorage.setItem(WALL.KEY,JSON.stringify({d:WALL.url,r:WALL.rep}));return true;}catch(e){return false;}}
function wallFromUrl(url,noSave,done){
  const im=new Image();
  im.onload=()=>{
    const k=Math.min(1,512/Math.max(im.width,im.height)),w=Math.max(1,Math.round(im.width*k)),h=Math.max(1,Math.round(im.height*k));
    const [c,x]=mkc(w,h);x.drawImage(im,0,0,w,h);
    if(WALL.tex&&WALL.tex.dispose)WALL.tex.dispose();
    WALL.tex=ctex(c,true);WALL.url=c.toDataURL('image/webp',.88);wallApplyRep();
    const ok=noSave||wallSave();
    if(done)done(ok);
  };
  im.onerror=()=>{if(done)done(false,true);};
  im.src=url;
}
function wallReset(){if(WALL.tex&&WALL.tex.dispose)WALL.tex.dispose();WALL.tex=null;WALL.url=null;try{localStorage.removeItem(WALL.KEY);}catch(e){}}
let HASGOAL=true,OPEN=false; /* OPEN : carte de parkour ouverte (sol seul, pas de murs ni de plafond) */
const HOOP={on:false,bw:7,bh:4.5,by:12.5,ry:9.5,rz:6.9,Rc:4.8,rt:.5};
const DROP={on:false,rad:10.5,tiles:[],ctx:null,tex:null,s:1,charge:0,team:-1};


/* =====================================================================
   DOM
   ===================================================================== */
const $=id=>document.getElementById(id);
const el={sB:$('sB'),sO:$('sO'),clock:$('clock'),ot:$('ot'),ring:$('ring'),bnum:$('bnum'),spd:$('spd'),feed:$('feed'),
  mbig:$('mbig'),msub:$('msub'),flash:$('flash'),hud:$('hud'),touch:$('touch'),rtag:$('rtag'),pause:$('pause'),end:$('end'),
  endT:$('endT'),endS:$('endS'),endTable:$('endTable')};
/* =====================================================================
   CATALOGUE : 100 voitures + roues, décalques, boosts, toppers, antennes,
   explosions, traînées, moteurs, hymnes, bannières, titres, finitions.
   Tout est généré par code (noms et modèles originaux).
   ===================================================================== */
function mulberry(a){return function(){a|=0;a=a+0x6D2B79F5|0;let t=Math.imul(a^a>>>15,1|a);t=t+Math.imul(t^t>>>7,61|t)^t;return((t^t>>>14)>>>0)/4294967296;};}
const RAR=[
  {k:'commun',n:'Commun',c:'#9aa5b8'},{k:'rare',n:'Rare',c:'#2f8bff'},{k:'tres',n:'Très rare',c:'#b967ff'},
  {k:'import',n:'Import',c:'#ff4d4d'},{k:'exo',n:'Exotique',c:'#ffd166'},{k:'noir',n:'Marché noir',c:'#ff4fd8'}
];
const CATS=[
  {k:'car',n:'Corps',ic:'🚗'},{k:'decal',n:'Décalques',ic:'🎨'},{k:'wheel',n:'Roues',ic:'🛞'},{k:'boost',n:'Boosts',ic:'🔥'},
  {k:'topper',n:'Toppers',ic:'🎩'},{k:'ant',n:'Antennes',ic:'📡'},{k:'expl',n:'Explosions',ic:'💥'},{k:'trail',n:'Traînées',ic:'✨'},
  {k:'eng',n:'Moteurs',ic:'🔊'},{k:'anth',n:'Hymnes',ic:'🎵'},{k:'ban',n:'Bannières',ic:'🏳️'},{k:'title',n:'Titres',ic:'🏷️'},
  {k:'fin',n:'Finitions',ic:'💎'}
];
const PRICE={
  car:[300,800,1600,3200,5500,9000],wheel:[100,300,700,1400,2500,4500],decal:[100,250,600,1200,2200,4000],
  boost:[150,350,800,1500,2800,5000],topper:[150,400,900,1800,3000,5000],ant:[100,250,500,1000,2000,3500],
  expl:[200,500,1000,2000,3500,6000],trail:[150,350,700,1400,2600,4500],eng:[100,300,600,1200,2200,4000],
  anth:[100,250,500,1000,2000,3500],ban:[50,150,300,600,1200,2200],title:[50,100,200,400,800,1500],
  fin:[100,300,600,1200,2500,5000]
};
const COLORS=[['Cobalt','#2f6bff'],['Azur','#3fa9ff'],['Cyan','#00e5ff'],['Turquoise','#19d3b0'],['Vert','#32d74b'],['Lime','#b6ff2e'],
  ['Jaune','#ffe14a'],['Or','#ffb400'],['Orange','#ff7a1a'],['Rouge','#ff3b3b'],['Carmin','#c2123a'],['Rose','#ff7ab8'],
  ['Magenta','#ff2bd6'],['Violet','#a24bff'],['Indigo','#4b3bd6'],['Blanc','#f4f7ff'],['Gris','#8a93a6'],['Noir','#171a22']];
const hexN=s=>parseInt(s.slice(1),16);

/* ---------- hitbox (comme dans le vrai jeu : plusieurs classes) ---------- */
const HB={
  std:{n:'Standard',hx:1.05,hy:.7,hz:2.15,r:1.85},
  dom:{n:'Allongé',hx:1.05,hy:.6,hz:2.4,r:1.95},
  plank:{n:'Plat',hx:1.15,hy:.55,hz:2.35,r:1.9},
  brk:{n:'Pointu',hx:1.0,hy:.6,hz:2.5,r:1.95},
  hyb:{n:'Compact',hx:1.05,hy:.75,hz:2.05,r:1.8},
  merc:{n:'Géant',hx:1.2,hy:.9,hz:2.2,r:2.05}
};

/* ---------- 12 silhouettes de base (x = avant, y = haut) ---------- */
const ARCH=[
  {n:'Coupé',hb:'std',w:2.0,wr:.55,sp:0,cv:'glass',body:[[-2.0,-.45],[2.0,-.45],[2.1,-.15],[1.45,.1],[.5,.22],[-1.6,.22],[-2.1,.02]],cab:[[-1.2,.2],[.6,.2],[.05,.82],[-.75,.82]]},
  {n:'Muscle',hb:'std',w:2.15,wr:.56,sp:1,cv:'glass',body:[[-2.1,-.45],[2.1,-.45],[2.25,-.2],[1.7,.08],[.6,.2],[-1.5,.2],[-2.15,.05]],cab:[[-1.0,.18],[.55,.18],[0,.72],[-.85,.72]]},
  {n:'Wedge',hb:'plank',w:1.95,wr:.52,sp:2,cv:'glass',body:[[-2.0,-.45],[2.1,-.45],[2.2,-.3],[.9,.05],[-.2,.22],[-1.7,.3],[-2.05,.1]],cab:[[-1.1,.25],[.2,.2],[-.2,.68],[-.9,.7]]},
  {n:'Compacte',hb:'hyb',w:1.95,wr:.55,sp:0,cv:'glass',body:[[-1.8,-.45],[1.8,-.45],[1.9,-.2],[1.3,.1],[.5,.25],[-1.5,.3],[-1.9,.15]],cab:[[-1.6,.28],[.45,.22],[.15,.95],[-1.3,.95]]},
  {n:'Berline',hb:'dom',w:2.05,wr:.55,sp:1,cv:'glass',body:[[-2.3,-.45],[2.3,-.45],[2.4,-.2],[1.7,.08],[.5,.22],[-1.9,.25],[-2.4,.08]],cab:[[-1.4,.22],[.5,.2],[-.1,.78],[-1.0,.8]]},
  {n:'Supercar',hb:'plank',w:2.2,wr:.5,sp:3,cv:'glass',body:[[-2.2,-.45],[2.3,-.45],[2.4,-.32],[1.2,-.05],[-.3,.12],[-1.9,.18],[-2.3,0]],cab:[[-1.0,.1],[.3,.08],[-.1,.52],[-.8,.54]]},
  {n:'Rallye',hb:'hyb',w:2.05,wr:.6,sp:3,cv:'glass',body:[[-1.9,-.45],[1.9,-.45],[2.0,-.15],[1.3,.15],[.3,.3],[-1.6,.3],[-2.0,.1]],cab:[[-1.4,.28],[.4,.28],[.1,1.0],[-1.1,1.02]]},
  {n:'Pick-up',hb:'merc',w:2.3,wr:.68,sp:0,cv:'glass',body:[[-2.1,-.35],[2.1,-.35],[2.2,-.05],[1.5,.25],[.7,.4],[-2.0,.4],[-2.15,.3]],cab:[[-.3,.38],[.8,.38],[.5,1.1],[-.15,1.1]]},
  {n:'Monoplace',hb:'brk',w:2.0,wr:.55,sp:4,cv:'open',body:[[-2.4,-.45],[2.5,-.45],[2.7,-.38],[1.8,-.12],[.2,0],[-2.0,.1],[-2.4,.05]],cab:[[-.6,0],[.3,0],[0,.45],[-.45,.45]]},
  {n:'Futuriste',hb:'brk',w:2.1,wr:.55,sp:5,cv:'bubble',body:[[-2.2,-.45],[2.6,-.45],[2.8,-.4],[1.4,0],[-.2,.18],[-1.9,.28],[-2.3,0]],cab:[[-1.0,.15],[.5,.1],[0,.62],[-.7,.65]]},
  {n:'Rétro',hb:'std',w:1.9,wr:.58,sp:0,cv:'glass',body:[[-1.9,-.45],[1.9,-.45],[2.0,-.2],[1.4,0],[.6,.12],[-1.5,.2],[-2.0,.05]],cab:[[-1.0,.15],[.35,.15],[0,.9],[-.8,.9]]},
  {n:'Fourgon',hb:'hyb',w:2.1,wr:.56,sp:0,cv:'glass',body:[[-1.7,-.45],[1.7,-.45],[1.8,-.1],[1.4,.2],[1.0,.35],[-1.6,.4],[-1.75,.3]],cab:[[-1.5,.38],[1.0,.35],[.8,1.0],[-1.4,1.05]]}
];
const CAR_NAMES=['Zyrex','Kobalt','Vortak','Nimbus','Quasar','Halcyon','Ferrox','Solstice','Mavrik','Onyx RS',
 'Drakon','Pulsar','Nomad GT','Cinder','Axiom','Helix','Rogue-9','Talon','Ember','Vanta',
 'Sirocco','Lynx R','Obsidian','Kestrel','Mirage X','Tundra','Warden','Blitzen','Echelon','Phoenix-7',
 'Zenith','Raze','Orbit','Havoc','Glacier','Specter','Boreal','Cyclops','Dynamo','Eclipse',
 'Frostbite','Gambit','Harbinger','Ignis','Javelin','Krypton-K','Lancer X','Maelstrom','Nebula-S','Osprey',
 'Paragon','Quicksilver','Ravager','Sabre','Titanis','Umbra','Valkyr','Wraith-R','Xenon','Yeti GT',
 'Zephyr','Aurora-X','Basilisk','Corsair','Delta-V','Equinox','Fury-S','Gryphon','Hydra','Inferno',
 'Juggernaut','Kodiak','Leviathan','Meteor','Nitro-N','Oracle','Panzer','Quake','Rampage','Stinger',
 'Tornado-T','Ultra-U','Vector','Whirl','Xylo','Yonder','Zodiac','Atlas-A','Bolt','Comet-C',
 'Dagger','Eon','Flare','Ghost-G','Hornet-H','Icarus','Jinx','Kraken','Lumen','Mythic'];

const WHEELS=[ // nom, style, nb, couleur jante, taille jante, largeur pneu
 ['Standard','spoke',5,'acc',.62,1],['Étoile','star',5,'wht',.64,1],['Turbine','turbine',8,'acc',.66,1],['Hexa','hex',6,'chr',.6,1.05],
 ['Disque','disc',0,'acc',.64,1],['Maille','mesh',12,'chr',.66,1],['Spirale','turbine',6,'neon',.66,1],['Lame','spoke',3,'acc',.64,1.1],
 ['Cyclone','turbine',10,'wht',.68,1],['Anneau','ring',4,'acc',.7,1],['Quadra','spoke',4,'blk',.6,1.1],['Penta','star',5,'gold',.64,1],
 ['Octo','spoke',8,'chr',.64,1],['Éclat','split',5,'acc',.66,1],['Rayon','mesh',16,'wht',.66,.95],['Vortex','turbine',12,'neon',.68,1],
 ['Orbite','ring',6,'gold',.7,1],['Fusion','split',6,'wht',.66,1],['Croix','spoke',2,'blk',.6,1.15],['Trident','spoke',3,'gold',.66,1],
 ['Nova','star',7,'neon',.66,1],['Delta','hex',6,'acc',.62,1.05],['Spider','split',8,'blk',.64,1],['Flux','turbine',5,'chr',.68,1],
 ['Pulse','disc',0,'neon',.66,1.1],['Tempête','mesh',10,'acc',.68,1],['Aigle','star',9,'gold',.66,1],['Néon','ring',8,'neon',.72,1],
 ['Chrome X','spoke',6,'chr',.64,1],['Atlas','disc',0,'gold',.62,1.15]
];
const DECALS=['Aucun','Bandes','Double bande','Flammes','Éclair','Damier','Tribal','Hexagones','Étoiles','Pois','Vagues','Camouflage',
 'Circuit','Griffes','Ailes','Numéro 7','Zébrure','Triangles','Écailles','Radar','Pixels','Graffiti','Racing','Dégradé'];
const TOPPERS=['Aucun','Haut-de-forme','Couronne','Casquette à hélice','Cône de chantier','Chapeau de fête','Chapeau de sorcier','Casque viking',
 'Cow-boy','Fedora','Tricorne','Auréole','Oreilles de chat','Oreilles de lapin','Crête punk','Casque audio','Trophée','Drapeau damier',
 'Cerise','Gâteau','Gyrophare','Hélicoptère','Planche de surf','Melon'];
const ANTS=['Aucune','Balle','Fanion','Drapeau','Cœur','Étoile','Éclair','Ballon','Épée','Palmier','Poisson','Fusée','Moulin','Carotte','Parabole','Sucette'];
const BOOSTS=[ // nom, c1, c2, type
 ['Feu standard','#ff8a1e','#cfe8ff','fire'],['Arc-en-ciel','#ff3b3b','#3bffd0','rainbow'],['Étincelles','#ffe14a','#ffffff','spark'],['Fumée noire','#444a58','#9aa3b5','smoke'],
 ['Bulles','#7fd8ff','#ffffff','bubble'],['Confettis','#ff4fd8','#ffe14a','confetti'],['Foudre','#7df9ff','#ffffff','bolt'],['Fantôme','#9dffcf','#e8fff4','ghost'],
 ['Pixels','#3bff5a','#c8ffd2','pixel'],['Neige','#e8f4ff','#ffffff','snow'],['Cœurs','#ff5fa8','#ffd0e6','heart'],['Magma','#ff3b1a','#ffb02e','fire'],
 ['Glace','#5fd0ff','#ffffff','spark'],['Galaxie','#a24bff','#ff6bdc','spark'],['Toxique','#9bff2e','#2e8f1a','smoke'],['Or massif','#ffcc33','#fff2b0','spark'],
 ['Plasma','#ff3bd0','#3b8bff','bolt'],['Matrice','#00ff66','#003b14','pixel'],['Soleil','#fff09a','#ff9a1a','fire'],['Néon violet','#c43bff','#ffffff','fire']
];
const EXPLS=[ // nom, motif, couleurs
 ['Classique','sphere',['t','#ffffff']],['Feu d\'artifice','firework',['t','#ffd166','#ff4fd8']],['Confettis','confetti',['#ff4fd8','#ffe14a','#3bffd0','#3b8bff']],
 ['Onde de choc','ring',['t','#ffffff']],['Foudre','bolt',['#7df9ff','#ffffff']],['Arc-en-ciel','spiral',['#ff3b3b','#ffb02e','#ffe14a','#32d74b','#3fa9ff','#a24bff']],
 ['Trou noir','implode',['#a24bff','#1a0a33']],['Pluie d\'étoiles','rain',['#ffe14a','#fff2b0']],['Fumée','smoke',['#444a58','#9aa3b5']],
 ['Comète','comet',['#7fd8ff','#ffffff']],['Tempête de neige','snow',['#e8f4ff','#9fd0ff']],['Cœurs','heart',['#ff5fa8','#ffd0e6']],
 ['Toxique','sphere',['#9bff2e','#2e8f1a']],['Supernova','nova',['t','#ffffff','#ffcc33']]
];
const TRAILS=[ // nom, type, couleurs
 ['Aucune','none',['#fff']],['Étincelles','spark',['#ffe14a','#ffffff']],['Arc-en-ciel','rainbow',[]],['Flammes','fire',['#ff8a1e','#ff3b1a']],
 ['Glace','spark',['#7fd8ff','#ffffff']],['Électrique','bolt',['#7df9ff','#ffffff']],['Fumée','smoke',['#7a8398','#444a58']],['Cœurs','heart',['#ff5fa8','#ffd0e6']],
 ['Étoiles','spark',['#fff2b0','#ffb02e']],['Bulles','bubble',['#9fe0ff','#ffffff']],['Matrice','pixel',['#00ff66','#003b14']],['Pastel','rainbow2',[]],
 ['Or','spark',['#ffcc33','#fff2b0']],['Ombre','smoke',['#1a1030','#6b3bd6']]
];
const ENGS=[ // nom, onde, base, k, coupure, onde2, ratio2
 ['Standard','sawtooth',48,3.4,420,'sine',1],['Turbo','square',54,3.8,520,'sawtooth',1.5],['V8 grave','sawtooth',36,2.6,300,'triangle',.5],['Électrique','sine',90,6,2000,'triangle',2],
 ['Moto','sawtooth',70,5,900,'square',2],['Synthé','square',60,4,700,'sine',1.01],['Réacteur','sawtooth',44,3,1300,'sawtooth',1.02],['Ronronnement','triangle',40,3,350,'sine',2],
 ['Rage','sawtooth',56,4.4,650,'square',.5],['Vaisseau','triangle',100,7,1600,'sine',3]
];
const ANTHS=[ // nom, notes [freq, début, durée]
 ['Fanfare',[[392,0,.15],[392,.18,.15],[523,.36,.5]]],['Victoire',[[523,0,.15],[659,.15,.15],[784,.3,.15],[1047,.45,.6]]],['Disco',[[330,0,.1],[330,.12,.1],[440,.24,.1],[330,.36,.1],[523,.48,.4]]],
 ['Alarme',[[880,0,.2],[660,.22,.2],[880,.44,.2],[660,.66,.3]]],['Arcade',[[262,0,.08],[330,.08,.08],[392,.16,.08],[523,.24,.08],[784,.32,.4]]],['Cosmique',[[220,0,.5],[330,.1,.5],[440,.2,.6],[660,.3,.8]]],
 ['Tambour',[[110,0,.12],[110,.2,.12],[110,.4,.12],[165,.55,.4]]],['Magie',[[784,0,.12],[988,.1,.12],[1175,.2,.12],[1568,.3,.6]]]
];
const BANS=[ // nom, fond CSS
 ['Standard','linear-gradient(90deg,#1c2748,#0e1530)'],['Océan','linear-gradient(90deg,#0a4a9a,#19d3b0)'],['Braise','linear-gradient(90deg,#8a1a00,#ff9a1a)'],['Forêt','linear-gradient(90deg,#0b5a2a,#7dff6b)'],
 ['Violet','linear-gradient(90deg,#3b0a7a,#ff4fd8)'],['Or','linear-gradient(90deg,#7a5200,#ffe066)'],['Glace','linear-gradient(90deg,#2a6a9a,#e8fbff)'],['Nuit','linear-gradient(90deg,#05070f,#2a2f5a)'],
 ['Rayures','repeating-linear-gradient(45deg,#1c2748 0 10px,#2f8bff 10px 20px)'],['Damier','repeating-conic-gradient(#fff 0 25%,#111 0 50%) 0 0/20px 20px'],['Aurore','linear-gradient(90deg,#19d3b0,#a24bff,#ff4fd8)'],['Lave','linear-gradient(90deg,#220000,#ff3b1a,#ffcc33)']
];
const TITLES=['Rookie','Recrue','Amateur','Pro','Vétéran','Légende','Buteur','Gardien','Aérien','Démolisseur','Roi du boost','Fantôme','Éclair','Tireur d\'élite','Mur d\'acier','Supersonique',
 'Maître du dribble','Champion','Grand champion','Imbattable','As du volant','Pilote fou','Tornade','Étoile montante','Machine','Chasseur','Roi de l\'arène','Sans pitié','Mythique','Légende vivante'];
const FINS=[['Mat',.15,.85],['Brillant',.35,.14],['Métallisé',.85,.25],['Anodisé',.9,.5],['Nacré',.55,.18],['Chromé',1,.05]];

/* ---------- construction des items ---------- */
const IT={},LST={};
function addItem(cat,i,name,r,extra){
  const it=Object.assign({id:cat+':'+i,cat:cat,i:i,n:name,r:r},extra||{});
  it.price=(i===0&&cat!=='car'&&cat!=='fin')||it.free?0:PRICE[cat][r];
  IT[it.id]=it;(LST[cat]=LST[cat]||[]).push(it);return it;
}
function rarityPlan(n,weights,seed){
  const arr=[];weights.forEach((w,r)=>{for(let k=0;k<w;k++)arr.push(r);});
  while(arr.length<n)arr.unshift(0);while(arr.length>n)arr.shift();
  const rg=mulberry(seed);for(let i=arr.length-1;i>0;i--){const j=(rg()*(i+1))|0;const t=arr[i];arr[i]=arr[j];arr[j]=t;}
  return arr;
}
function w6(n){ // répartition type du vrai jeu : beaucoup de communs, peu de rares
  const a=[.36,.26,.17,.11,.07,.03].map(x=>Math.round(x*n));return a;
}
{
  const rp=rarityPlan(100,[32,26,18,12,8,4],11);
  for(let i=0;i<6;i++)rp[i]=0;
  for(let i=0;i<100;i++){const a=ARCH[i%12];addItem('car',i,CAR_NAMES[i],rp[i],{free:i<6,sub:a.n+' • Hitbox '+HB[a.hb].n,hb:a.hb});}
  const add=(cat,names,seed,fn,freeN)=>{
    const rp2=rarityPlan(names.length,w6(names.length),seed);
    names.forEach((nm,i)=>{const o=fn?fn(i,nm):{};if(i>0&&i<(freeN||1))o.free=true;addItem(cat,i,Array.isArray(nm)?nm[0]:nm,i===0?0:rp2[i],o);});
  };
  add('decal',DECALS,21,null,2);
  add('wheel',WHEELS,22,(i)=>({wd:{st:WHEELS[i][1],n:WHEELS[i][2],cm:WHEELS[i][3],rs:WHEELS[i][4],tw:WHEELS[i][5]}}),3);
  add('boost',BOOSTS,23,(i)=>({bd:{c1:BOOSTS[i][1],c2:BOOSTS[i][2],k:BOOSTS[i][3]}}),2);
  add('topper',TOPPERS,24,null,1);
  add('ant',ANTS,25,null,1);
  add('expl',EXPLS,26,(i)=>({ed:{pat:EXPLS[i][1],col:EXPLS[i][2]}}),2);
  add('trail',TRAILS,27,(i)=>({td:{k:TRAILS[i][1],col:TRAILS[i][2]}}),1);
  add('eng',ENGS,28,(i)=>({ad:{t1:ENGS[i][1],base:ENGS[i][2],k:ENGS[i][3],cut:ENGS[i][4],t2:ENGS[i][5],r2:ENGS[i][6]}}),1);
  add('anth',ANTHS,29,(i)=>({nd:ANTHS[i][1]}),1);
  add('ban',BANS,30,(i)=>({css:BANS[i][1]}),2);
  add('title',TITLES,31,null,2);
  add('fin',FINS,32,(i)=>({fd:{m:FINS[i][1],r:FINS[i][2]}}),2);
}
// le premier élément de chaque catégorie "aucun" garde un nom parlant
IT['title:0'].n='Rookie';
// Les finitions 0 et 1 sont offertes
IT['fin:0'].price=0;IT['fin:1'].price=0;

/* ---------- paramètres de chaque voiture (silhouette + détails) ---------- */
const CARP={};
function carParams(i){
  if(CARP[i])return CARP[i];
  const a=ARCH[i%12],v=(i/12)|0,rg=mulberry(1000+i*77),j=v===0?0:1;
  const sx=1+(rg()-.5)*.24*j,sy=1+(rg()-.5)*.34*j,ws=1+(rg()-.5)*.14*j;
  const tx=p=>[p[0]*sx,-.45+(p[1]+.45)*sy];
  const body=a.body.map(tx),cab=a.cab.map(tx);
  let mn=1e9,mx=-1e9;body.forEach(p=>{mn=Math.min(mn,p[0]);mx=Math.max(mx,p[0]);});
  const hc=[0xfff3c0,0xffe066,0x9be7ff,0xffa0e0,0xff8a8a,0xa0ffb0],tc=[0xff2a2a,0xff7a1a,0xff2bd6,0x2bd6ff];
  const P={
    body:body,cab:cab,w:+(a.w*ws).toFixed(3),wr:a.wr,wb:Math.max(1.05,Math.min(1.55,(mx-mn)*.3)),xmin:mn,xmax:mx,
    sp:rg()<.45?((rg()*6)|0):a.sp,split:rg()<.35,scoop:rg()<.3,rscoop:rg()<.2,fins:rg()<.2,pods:rg()<.35,flare:rg()<.3,
    exh:(rg()*4)|0,lbar:rg()<.18,head:hc[(rg()*hc.length)|0],tail:tc[(rg()*tc.length)|0],
    cv:rg()<.12&&a.cv==='glass'?'bubble':a.cv,tint:[0x0a1020,0x101820,0x201010,0x102010,0x180a28][(rg()*5)|0],hb:a.hb,arch:a.n
  };
  CARP[i]=P;return P;
}

/* ---------- sauvegarde ---------- */
const SAVE_KEY='rocketArena3D.save.v2';
let SV=null;
function defLoad(){return {car:0,decal:0,wheel:0,boost:0,topper:0,ant:0,expl:0,trail:0,eng:0,anth:0,ban:0,title:0,fin:1,c1:0,c2:7};}
function freshSave(){
  return {name:'Joueur',credits:3000,xp:0,owned:{},load:defLoad(),presets:[null,null,null,null],preset:0,
    stats:{matches:0,wins:0,goals:0,assists:0,saves:0,shots:0,demos:0,playT:0},
    daily:{last:'',streak:0},mmr:{'1':520,'2':520,'3':520},plc:{},ch:{day:'',prog:{},claimed:{}},pass:{claimed:0},
    set:{vol:.55,ballCam:1,unlockAll:false,autoThr:1,camF:78,camD:11.5,camH:4.4,camS:10},menu:{mode:'casual',size:2,diff:1,dur:3,team:0,mut:{},multi:[],exMode:'casual',ws:0},crates:0,tour:null,mv:{},nd:null,ndStart:0,ch2:null,arc:null,season:0,lg:null};
}
function loadSV(){
  const f=freshSave();let s=null;
  try{s=JSON.parse(localStorage.getItem(SAVE_KEY)||'null');}catch(e){s=null;}
  if(s&&typeof s==='object'){
    for(const k in f){if(s[k]===undefined)continue;
      if(f[k]&&typeof f[k]==='object'&&!Array.isArray(f[k])&&s[k]&&typeof s[k]==='object')f[k]=Object.assign(f[k],s[k]);else f[k]=s[k];}
    if(!Array.isArray(f.presets))f.presets=[null,null,null,null];
  }
  return f;
}
function saveSV(){try{localStorage.setItem(SAVE_KEY,JSON.stringify(SV));}catch(e){}try{LB.schedule();}catch(e){}try{CLOUD.schedule();}catch(e){}}
SV=loadSV();
const mmrOf=k=>SV.mmr[k]!==undefined?SV.mmr[k]:(SV.ndStart||520);
const owns=id=>{const it=IT[id];return !!it&&(SV.set.unlockAll||it.price===0||!!SV.owned[id]);};
const lvlOf=xp=>Math.floor(xp/500)+1;
function todayKey(){const d=new Date();return d.getFullYear()+'-'+(d.getMonth()+1)+'-'+d.getDate();}
function dayNum(){return Math.floor(Date.now()/86400000);}

/* ---------- loadouts aléatoires pour les bots ---------- */
function randLoad(rg){
  const L=defLoad();
  L.car=(rg()*100)|0;
  for(const c of ['decal','wheel','boost','topper','ant','expl','trail','eng','anth','ban','title']){
    const n=LST[c].length;L[c]=rg()<.3&&c!=='wheel'&&c!=='boost'&&c!=='eng'?0:((rg()*n)|0);
  }
  L.fin=(rg()*FINS.length)|0;L.c1=(rg()*COLORS.length)|0;L.c2=(rg()*COLORS.length)|0;
  return L;
}


/* =====================================================================
   RENDU / SCÈNE
   ===================================================================== */
const canvas=$('c');
const renderer=new T.WebGLRenderer({canvas:canvas,antialias:(window.devicePixelRatio||1)<2.5,powerPreference:'high-performance'});
renderer.setPixelRatio(Math.min(window.devicePixelRatio||1,2));
const scene=new T.Scene();
scene.background=new T.Color(0x070b1a);
scene.fog=new T.Fog(0x070b1a,260,800);
const camera=new T.PerspectiveCamera(72,1,0.5,2000);
let baseFov=78;
function resize(){
  const w=window.innerWidth,h=window.innerHeight;
  renderer.setSize(w,h,false);
  camera.aspect=w/h;
  baseFov=camera.aspect<1?92:78;
  camera.updateProjectionMatrix();
}
window.addEventListener('resize',resize);resize();

function mkc(w,h){const c=document.createElement('canvas');c.width=w;c.height=h;return [c,c.getContext('2d')];}
function ctex(c,rep){const t=new T.CanvasTexture(c);if(rep){t.wrapS=t.wrapT=T.RepeatWrapping;}t.anisotropy=Math.min(8,renderer.capabilities.getMaxAnisotropy?renderer.capabilities.getMaxAnisotropy():1);return t;}
function hexPath(x,cx,cy,r){x.beginPath();for(let k=0;k<6;k++){const a=k*Math.PI/3;const px=cx+Math.cos(a)*r,py=cy+Math.sin(a)*r;k?x.lineTo(px,py):x.moveTo(px,py);}x.closePath();}

/* ---------- textures ---------- */
function floorTex(kind){
  kind=kind||'std';
  const W=kind==='drop'?2048:1024,Hh=Math.round(W*HZ/HX),[c,x]=mkc(W,Hh),s=W/(2*HX);
  const X=u=>(u+HX)*s,Z=u=>(u+HZ)*s;
  const PAL={std:['#1d8f3f','#168030','rgba(255,255,255,.92)'],snow:['#d9edf8','#c5e1f2','rgba(70,140,200,.9)'],hoops:['#c98543','#b87636','rgba(255,255,255,.92)'],drop:['#0b1226','#0b1226','rgba(140,180,255,.6)']};
  const pal=PAL[kind]||PAL.std;
  x.fillStyle='#0b1226';x.fillRect(0,0,W,Hh);
  const fx0=X(-(HX-R)),fz0=Z(-(HZ-R)),fw=2*(HX-R)*s,fh=2*(HZ-R)*s,st=10*s;
  if(kind==='hoops'){const pw=2.4*s;for(let i=0;i*pw<fw;i++){x.fillStyle=i%2?pal[0]:pal[1];x.fillRect(fx0+i*pw,fz0,Math.min(pw,fw-i*pw)+1,fh);}}
  else for(let i=0;i*st<fh;i++){x.fillStyle=i%2?pal[0]:pal[1];x.fillRect(fx0,fz0+i*st,fw,Math.min(st,fh-i*st)+1);}
  for(let i=0;i<30000;i++){x.fillStyle=rnd()<.5?'rgba(255,255,255,.04)':'rgba(0,0,0,.08)';x.fillRect(fx0+rnd()*fw,fz0+rnd()*fh,1+rnd()*3,1+rnd()*5);}
  let g=x.createLinearGradient(0,fz0,0,fz0+fh*.38);g.addColorStop(0,'rgba(47,139,255,'+(kind==='snow'?.22:.45)+')');g.addColorStop(1,'rgba(47,139,255,0)');x.fillStyle=g;x.fillRect(fx0,fz0,fw,fh*.38);
  g=x.createLinearGradient(0,fz0+fh,0,fz0+fh*.62);g.addColorStop(0,'rgba(255,138,42,'+(kind==='snow'?.22:.45)+')');g.addColorStop(1,'rgba(255,138,42,0)');x.fillStyle=g;x.fillRect(fx0,fz0+fh*.62,fw,fh*.38);
  x.strokeStyle=pal[2];x.lineWidth=5;x.lineJoin='round';
  if(kind==='drop'){
    DROP.ctx=x;DROP.s=s;DROP.X=X;DROP.Z=Z;
    DROP.tiles.forEach(t=>dropDrawTile(t));
    x.lineWidth=16;x.strokeStyle='rgba(120,190,255,.18)';x.beginPath();for(let k=0;k<6;k++){const a=k*Math.PI/3,rr=(HZ-R)/.8660254+2,qx=X(Math.cos(a)*rr),qz=Z(Math.sin(a)*rr);k?x.lineTo(qx,qz):x.moveTo(qx,qz);}x.closePath();x.stroke();
    DROP.tex=ctex(c,false);return DROP.tex;
  }
  x.strokeRect(fx0,fz0,fw,fh);
  x.beginPath();x.moveTo(fx0,Z(0));x.lineTo(fx0+fw,Z(0));x.stroke();
  x.beginPath();x.arc(X(0),Z(0),(kind==='hoops'?7:12)*s,0,Math.PI*2);x.stroke();
  x.fillStyle=pal[2];x.beginPath();x.arc(X(0),Z(0),1.1*s,0,Math.PI*2);x.fill();
  if(kind==='hoops'){
    for(const sg of [-1,1]){
      x.strokeRect(X(-8),sg<0?Z(-(HZ-R)):Z((HZ-R)-14),16*s,14*s);
      x.beginPath();x.arc(X(0),Z(sg*(HZ-R-14)),8*s,sg<0?0:Math.PI,sg<0?Math.PI:Math.PI*2,false);x.stroke();
      x.beginPath();x.arc(X(0),Z(sg*(HZ-HOOP.rz)),HOOP.Rc*s,0,Math.PI*2);x.stroke();
    }
  }else{
    for(const sg of [-1,1]){
      x.strokeRect(X(-20),sg<0?Z(-(HZ-R)):Z((HZ-R)-16),40*s,16*s);
      x.strokeRect(X(-10),sg<0?Z(-(HZ-R)):Z((HZ-R)-6),20*s,6*s);
      x.beginPath();x.arc(X(0),Z(sg*(HZ-R-12)),1*s,0,Math.PI*2);x.fill();
    }
  }
  x.lineWidth=16;x.strokeStyle='rgba(120,190,255,.18)';x.strokeRect(fx0-6,fz0-6,fw+12,fh+12);
  return ctex(c,false);
}

function wallTex(){
  const [c,x]=mkc(576,554);
  x.clearRect(0,0,576,554);
  const r=32,hh=554/10;
  for(let col=-1;col<=12;col++)for(let row=-1;row<=10;row++){
    const cx=col*r*1.5,cy=row*hh+(((col%2)+2)%2?hh/2:0);
    hexPath(x,cx,cy,r-2);
    const lit=rnd()<.12;
    x.fillStyle=lit?'rgba(150,205,255,.42)':'rgba(30,60,120,.22)';x.fill();
    x.lineWidth=3;x.strokeStyle='rgba(170,215,255,.9)';x.stroke();
  }
  return ctex(c,true);
}
function ballTex(){
  const [c,x]=mkc(512,256);
  x.fillStyle='#16233f';x.fillRect(0,0,512,256);
  const r=30,hh=Math.sqrt(3)*r;
  for(let col=-1;col<=18;col++)for(let row=-1;row<=5;row++){
    const cx=col*r*1.5,cy=row*hh+(((col%2)+2)%2?hh/2:0);
    hexPath(x,cx,cy,r-3);x.fillStyle=rnd()<.2?'#79c6ff':'#eef4ff';x.fill();
    x.lineWidth=3;x.strokeStyle='#2a6bff';x.stroke();
  }
  return ctex(c,true);
}
function netTex(){
  const [c,x]=mkc(128,128);
  x.clearRect(0,0,128,128);x.strokeStyle='rgba(255,255,255,.95)';x.lineWidth=4;
  x.strokeRect(0,0,128,128);x.beginPath();x.moveTo(0,64);x.lineTo(128,64);x.moveTo(64,0);x.lineTo(64,128);x.stroke();
  return ctex(c,true);
}
function glowTex(){
  const [c,x]=mkc(64,64);const g=x.createRadialGradient(32,32,0,32,32,32);
  g.addColorStop(0,'rgba(255,255,255,1)');g.addColorStop(.35,'rgba(255,255,255,.55)');g.addColorStop(1,'rgba(255,255,255,0)');
  x.fillStyle=g;x.fillRect(0,0,64,64);return new T.CanvasTexture(c);
}
function padTexMk(){
  const [c,x]=mkc(128,128);const g=x.createRadialGradient(64,64,4,64,64,62);
  g.addColorStop(0,'rgba(255,190,80,.9)');g.addColorStop(.5,'rgba(255,140,30,.35)');g.addColorStop(1,'rgba(255,120,20,0)');
  x.fillStyle=g;x.fillRect(0,0,128,128);x.strokeStyle='rgba(255,225,150,.95)';x.lineWidth=5;hexPath(x,64,64,40);x.stroke();
  return new T.CanvasTexture(c);
}
function skyTex(){
  const [c,x]=mkc(1024,512);const g=x.createLinearGradient(0,0,0,512);
  g.addColorStop(0,'#02030a');g.addColorStop(.45,'#0a1230');g.addColorStop(.52,'#1d2c63');g.addColorStop(.56,'#3a2a5a');g.addColorStop(1,'#05070f');
  x.fillStyle=g;x.fillRect(0,0,1024,512);
  for(let i=0;i<600;i++){x.fillStyle='rgba(255,255,255,'+(rnd()*.8)+')';const s=rnd()*1.8;x.fillRect(rnd()*1024,rnd()*250,s,s);}
  return new T.CanvasTexture(c);
}
const TX={floor:floorTex(),wall:wallTex(),ball:ballTex(),net:netTex(),glow:glowTex(),pad:padTexMk()};

/* ---------- ciel, lumières, reflets ---------- */
{
  const sky=new T.Mesh(new T.SphereGeometry(900,24,16),new T.MeshBasicMaterial({map:skyTex(),side:T.BackSide,fog:false}));
  scene.add(sky);
  scene.add(new T.HemisphereLight(0xc5dcff,0x1a2238,.85));
  const sun=new T.DirectionalLight(0xffffff,.75);sun.position.set(25,70,15);scene.add(sun);
  const pb=new T.PointLight(0x2f8bff,.9,90);pb.position.set(0,16,-HZ+10);scene.add(pb);
  const po=new T.PointLight(0xff8a2a,.9,90);po.position.set(0,16,HZ-10);scene.add(po);
  try{
    const [c,x]=mkc(512,256);const g=x.createLinearGradient(0,0,0,256);
    g.addColorStop(0,'#bcd6ff');g.addColorStop(.45,'#2b4f9a');g.addColorStop(.5,'#101a38');g.addColorStop(1,'#05070f');
    x.fillStyle=g;x.fillRect(0,0,512,256);x.fillStyle='#fff';
    for(let i=0;i<14;i++)x.fillRect(rnd()*512,20+rnd()*80,30+rnd()*60,7);
    const et=new T.CanvasTexture(c);et.mapping=T.EquirectangularReflectionMapping;
    const pm=new T.PMREMGenerator(renderer);scene.environment=pm.fromEquirectangular(et).texture;pm.dispose();
  }catch(e){}
}

/* =====================================================================
   GÉOMÉTRIE DE L'ARÈNE : champ de distance signé (physique) + maillage
   ===================================================================== */
function rbox(px,py,pz,cx,cy,cz,hx,hy,hz,r,o){
  const dx=px-cx,dy=py-cy,dz=pz-cz;
  const qx=Math.abs(dx)-(hx-r),qy=Math.abs(dy)-(hy-r),qz=Math.abs(dz)-(hz-r);
  const mx=qx>0?qx:0,my=qy>0?qy:0,mz=qz>0?qz:0;
  const l=Math.sqrt(mx*mx+my*my+mz*mz);
  if(l>1e-9){o.d=l-r;o.nx=mx/l*(dx<0?-1:1);o.ny=my/l*(dy<0?-1:1);o.nz=mz/l*(dz<0?-1:1);}
  else{
    o.d=Math.max(qx,qy,qz)-r;
    if(qx>=qy&&qx>=qz){o.nx=dx<0?-1:1;o.ny=0;o.nz=0;}
    else if(qy>=qz){o.nx=0;o.ny=dy<0?-1:1;o.nz=0;}
    else{o.nx=0;o.ny=0;o.nz=dz<0?-1:1;}
  }
}
const S={d:0,nx:0,ny:0,nz:0},S1={d:0,nx:0,ny:0,nz:0};
let GCZ=HZ+(GD-R)/2,GHZ=(GD+R)/2;
function torusD(px,py,pz,cx,cy,cz,Rc,rt,o){
  const dx=px-cx,dz=pz-cz,dy=py-cy,dl=Math.hypot(dx,dz)||1e-6;
  const qx=dl-Rc,l=Math.hypot(qx,dy)||1e-6;
  o.d=l-rt;const ux=qx/l,uy=dy/l;o.nx=ux*dx/dl;o.ny=uy;o.nz=ux*dz/dl;
}
function cutSolid(o){ /* S1 décrit un solide : on le retire de l'espace libre */
  if(-S1.d>o.d){o.d=-S1.d;o.nx=-S1.nx;o.ny=-S1.ny;o.nz=-S1.nz;}
}
/* ---------- Dropshot : arène hexagonale (prisme hexagonal à bords arrondis) ---------- */
const HEX={N:[],C:[],S:[]},HC={x:0,z:0,dd:0,nx:0,nz:0};
for(let i=0;i<6;i++){const a=(30+60*i)*Math.PI/180;HEX.N.push([Math.cos(a),Math.sin(a)]);HEX.C.push(Math.cos(i*Math.PI/3));HEX.S.push(Math.sin(i*Math.PI/3));}
HEX.C.push(HEX.C[0]);HEX.S.push(HEX.S[0]);
/* point de l'hexagone (apothème ai) le plus proche de (x,z) ; HC.dd = distance signée (<0 dedans), HC.nx/nz = normale sortante */
function hexClosest(x,z,ai){
  const N=HEX.N;let m=-1e9,mi=0;
  for(let i=0;i<6;i++){const d=x*N[i][0]+z*N[i][1]-ai;if(d>m){m=d;mi=i;}}
  if(m<=0){HC.x=x;HC.z=z;HC.dd=m;HC.nx=N[mi][0];HC.nz=N[mi][1];return;}
  const ri=ai/.8660254;let best=1e18,bx=0,bz=0;
  for(let i=0;i<6;i++){
    if(x*N[i][0]+z*N[i][1]-ai<=0)continue;
    const ax=HEX.C[i]*ri,az=HEX.S[i]*ri,ex=HEX.C[i+1]*ri-ax,ez=HEX.S[i+1]*ri-az;
    let t=((x-ax)*ex+(z-az)*ez)/(ex*ex+ez*ez);t=t<0?0:t>1?1:t;
    const qx=ax+ex*t,qz=az+ez*t,d2=(x-qx)*(x-qx)+(z-qz)*(z-qz);
    if(d2<best){best=d2;bx=qx;bz=qz;}
  }
  const dd=Math.sqrt(best)||1e-9;
  HC.x=bx;HC.z=bz;HC.dd=dd;HC.nx=(x-bx)/dd;HC.nz=(z-bz)/dd;
}
function hexSdf(px,py,pz,o){
  const hh=HY-R,dy=py-HY,ady=Math.abs(dy)-hh;
  hexClosest(px,pz,HZ-R);
  const a=HC.dd;
  if(a<=0&&ady<=0){
    if(a>=ady){o.d=a-R;o.nx=HC.nx;o.ny=0;o.nz=HC.nz;}
    else{o.d=ady-R;o.nx=0;o.ny=dy<0?-1:1;o.nz=0;}
  }else{
    const a2=a>0?a:0,b2=ady>0?ady:0,l=Math.sqrt(a2*a2+b2*b2);
    o.d=l-R;
    if(a2>0){o.nx=HC.nx*a2/l;o.nz=HC.nz*a2/l;}else{o.nx=0;o.nz=0;}
    o.ny=b2>0?(dy<0?-1:1)*b2/l:0;
  }
}
/* ramène un point (v.x, v.z) dans la zone plate de l'hexagone, avec une marge m */
function hexFit(v,m){hexClosest(v.x,v.z,HZ-R-m);if(HC.dd>0){v.x=HC.x;v.z=HC.z;}}
/* le point est-il sur le sol plat (hors rampes) ? */
function inFlat(x,z,m){if(DROP.on){hexClosest(x,z,HZ-m);return HC.dd<0;}return Math.abs(x)<HX-m&&Math.abs(z)<HZ-m;}
function dropHole(px,pz){
  const t=DROP.tiles,ap=DROP.rad*.8660254;
  for(let i=0;i<t.length;i++){const a=t[i];if(a.st<2)continue; /* 2 étapes : fissurée (1) = encore pleine, détruite (2) = trou où la balle tombe */const dx=Math.abs(px-a.x),dz=Math.abs(pz-a.z);if(dx<=ap&&dz+dx*.57735<=DROP.rad)return true;}
  return false;
}
function sdf(px,py,pz,o,forBall){
  if(OPEN){o.nx=0;o.ny=-1;o.nz=0;o.d=(Math.abs(px)<HX&&Math.abs(pz)<HZ)?-py:-1000;} /* sol seul ; hors du sol : le vide */
  else if(DROP.on)hexSdf(px,py,pz,o);
  else rbox(px,py,pz,0,HY,0,HX,HY,HZ,R,o);
  if(HASGOAL){
    rbox(px,py,pz,0,GH/2,GCZ,GW,GH/2,GHZ,1,S1);
    if(S1.d<o.d){o.d=S1.d;o.nx=S1.nx;o.ny=S1.ny;o.nz=S1.nz;}
    rbox(px,py,pz,0,GH/2,-GCZ,GW,GH/2,GHZ,1,S1);
    if(S1.d<o.d){o.d=S1.d;o.nx=S1.nx;o.ny=S1.ny;o.nz=S1.nz;}
  }
  if(HOOP.on){
    for(let s=-1;s<=1;s+=2){
      rbox(px,py,pz,0,HOOP.by,s*(HZ-.6),HOOP.bw,HOOP.bh,.6,.25,S1);cutSolid(o);
      torusD(px,py,pz,0,HOOP.ry,s*(HZ-HOOP.rz),HOOP.Rc,HOOP.rt,S1);cutSolid(o);
    }
  }
  if(CM.cur&&CM.cur.obs.length)cmSdf(px,py,pz,o);
  if(forBall&&DROP.on&&o.ny<-.7&&py<R+4&&dropHole(px,pz))o.d=-60;
}

const CO={};
function axisC(h,extras){
  const a=[],K=8;
  for(let i=0;i<=K;i++){const t=i/K*R;a.push(-h+t,h-t);}
  for(const e of extras)a.push(e);
  a.sort((p,q)=>p-q);
  const o=[a[0]];for(let i=1;i<a.length;i++)if(a[i]-o[o.length-1]>1e-4)o.push(a[i]);
  return o;
}
function recomputeCO(){const ex=h=>{const a=[];for(let v=-Math.floor((h-1)/10)*10;v<h;v+=10)a.push(v);return a;};CO.x=axisC(HX,[-GW,GW].concat(ex(HX)));CO.y=axisC(HY,[GH-HY,0,-HY+3]);CO.z=axisC(HZ,ex(HZ));}
recomputeCO();

function tintAt(z){
  const s=clamp(z/HZ,-1,1),k=clamp(Math.pow(Math.abs(s),1.1)*1.35,0,1);
  const base=[.72,.82,1],tc=s<0?[.15,.5,1]:[1,.5,.12];
  return [lerp(base[0],tc[0],k),lerp(base[1],tc[1],k),lerp(base[2],tc[2],k)];
}

function buildArenaHex(){
  const P=[],N=[],Cc=[],U=[],idx=[[],[],[]];
  const A=HZ,ai=A-R,L=A/.8660254;
  /* chaque point de la coque extérieure est ramené sur la surface arrondie (même méthode que l'arène rectangulaire) */
  const vert=(ox,oy,oz,m,fl,tx,tz)=>{
    hexClosest(ox,oz,ai);
    const hx=HC.x,hz=HC.z,iy=clamp(oy,-(HY-R),HY-R);
    let dx=ox-hx,dy=oy-iy,dz=oz-hz;const l=Math.sqrt(dx*dx+dy*dy+dz*dz)||1;dx/=l;dy/=l;dz/=l;
    const wx=hx+dx*R,wy=iy+dy*R+HY,wz=hz+dz*R;
    P.push(wx,wy,wz);N.push(dx,dy,dz);
    if(m===1){const t=tintAt(wz),sh=clamp(.82-.28*dy,.5,1.05);Cc.push(t[0]*sh,t[1]*sh,t[2]*sh);U.push(fl?wx/28.8:(wx*tx+wz*tz)/28.8,fl?wz/27.7:wy/27.7);}
    else{Cc.push(1,1,1);if(m===0)U.push((wx+HX)/(2*HX),1-(wz+HZ)/(2*HZ));else U.push(wx/28.8,wz/27.7);}
    return P.length/3-1;
  };
  /* 6 murs (rampes et coins arrondis compris) */
  const ex=[];for(let v=-Math.floor((L/2-1)/10)*10;v<L/2;v+=10)ex.push(v);
  const UA=axisC(L/2,ex),VA=CO.y;
  for(let e=0;e<6;e++){
    const ph=(30+60*e)*Math.PI/180,nx=Math.cos(ph),nz=Math.sin(ph),tx=-nz,tz=nx;
    const pt=(a,b)=>vert(nx*A+tx*UA[a],VA[b],nz*A+tz*UA[a],1,false,tx,tz);
    for(let j=0;j<VA.length-1;j++)for(let i=0;i<UA.length-1;i++){
      const a=pt(i,j),b=pt(i+1,j),c=pt(i+1,j+1),d=pt(i,j+1);
      idx[1].push(a,b,c,a,c,d);
    }
  }
  /* sol et plafond plats : 6 secteurs triangulaires, du bord des rampes jusqu'au centre */
  const SR=[R],nst=Math.ceil((A-R)/10);for(let k=1;k<=nst;k++)SR.push(R+(A-R)*k/nst);
  const UC=[0,.2,.4,.6,.8,1];
  for(const [oy,m] of [[-HY,0],[HY,2]]){
    for(let e=0;e<6;e++){
      const W0x=HEX.C[e]*L,W0z=HEX.S[e]*L,W1x=HEX.C[e+1]*L,W1z=HEX.S[e+1]*L;
      const pt=(i,k)=>{const f=1-SR[k]/A,u=UC[i];return vert((W0x+(W1x-W0x)*u)*f,oy,(W0z+(W1z-W0z)*u)*f,m,true,0,0);};
      for(let k=0;k<SR.length-1;k++)for(let i=0;i<UC.length-1;i++){
        const a=pt(i,k),b=pt(i+1,k),c=pt(i+1,k+1),d=pt(i,k+1);
        idx[m].push(a,b,c,a,c,d);
      }
    }
  }
  const g=new T.BufferGeometry();
  g.setAttribute('position',new T.Float32BufferAttribute(P,3));
  g.setAttribute('normal',new T.Float32BufferAttribute(N,3));
  g.setAttribute('color',new T.Float32BufferAttribute(Cc,3));
  g.setAttribute('uv',new T.Float32BufferAttribute(U,2));
  const all=[];let st=0;
  for(let m=0;m<3;m++){if(idx[m].length){g.addGroup(st,idx[m].length,m);for(const v of idx[m])all.push(v);st+=idx[m].length;}else g.addGroup(st,0,m);}
  g.setIndex(all);
  const floorMat=new T.MeshStandardMaterial({map:ARN.tex,roughness:.72,metalness:.05,side:T.DoubleSide,alphaTest:.5});
  const wtx=ARN.wtex||WALL.tex,wallMat=new T.MeshBasicMaterial({map:wtx||TX.wall,vertexColors:!wtx,transparent:true,depthWrite:false,side:T.DoubleSide});
  const ceilMat=new T.MeshBasicMaterial({map:TX.wall,color:0x6fa8ff,transparent:true,opacity:.16,depthWrite:false,side:T.DoubleSide});
  arenaGroup.add(new T.Mesh(g,[floorMat,wallMat,ceilMat]));
}
function buildArena(){
  if(DROP.on){buildArenaHex();return;}
  const P=[],N=[],Cc=[],U=[],idx=[[],[],[]],cache=new Map();
  const faces=[['x',1],['x',-1],['y',1],['y',-1],['z',1],['z',-1]];
  const ORD={'+x':['y','z'],'-x':['z','y'],'+y':['z','x'],'-y':['x','z'],'+z':['x','y'],'-z':['y','x']};
  faces.forEach((f,fi)=>{
    const an=f[0],sg=f[1],ord=ORD[(sg>0?'+':'-')+an],a1=ord[0],a2=ord[1],A=CO[a1],B=CO[a2];
    const hv=({x:HX,y:HY,z:HZ})[an]*sg;
    const vert=(i,j,m)=>{
      const key=fi+'|'+i+'|'+j+'|'+m;let v=cache.get(key);if(v!==undefined)return v;
      const o={x:0,y:0,z:0};o[an]=hv;o[a1]=A[i];o[a2]=B[j];
      const ix=clamp(o.x,-(HX-R),HX-R),iy=clamp(o.y,-(HY-R),HY-R),iz=clamp(o.z,-(HZ-R),HZ-R);
      let dx=o.x-ix,dy=o.y-iy,dz=o.z-iz;const l=Math.sqrt(dx*dx+dy*dy+dz*dz);dx/=l;dy/=l;dz/=l;
      const wx=ix+dx*R,wy=iy+dy*R+HY,wz=iz+dz*R;
      v=P.length/3;P.push(wx,wy,wz);N.push(dx,dy,dz);
      if(m===1){
        const t=tintAt(wz),sh=clamp(.82-.28*dy,.5,1.05);Cc.push(t[0]*sh,t[1]*sh,t[2]*sh);
        if(an==='x')U.push(wz/28.8,wy/27.7);else if(an==='z')U.push(wx/28.8,wy/27.7);else U.push(wx/28.8,wz/27.7);
      }else{
        Cc.push(1,1,1);
        if(m===0)U.push((wx+HX)/(2*HX),1-(wz+HZ)/(2*HZ));else U.push(wx/28.8,wz/27.7);
      }
      cache.set(key,v);return v;
    };
    for(let j=0;j<B.length-1;j++)for(let i=0;i<A.length-1;i++){
      const c={x:0,y:0,z:0};c[an]=hv;c[a1]=(A[i]+A[i+1])/2;c[a2]=(B[j]+B[j+1])/2;
      let m=1;
      if(an==='y'){
        const flat=Math.abs(c.x)<HX-R&&Math.abs(c.z)<HZ-R;
        if(sg<0){if(Math.abs(c.x)<GW&&Math.abs(c.z)>HZ-R)continue;m=flat?0:1;}
        else m=flat?2:1;
      }else if(an==='z'){
        if(Math.abs(c.x)<GW&&(c.y+HY)<GH)continue;
      }
      const a=vert(i,j,m),b=vert(i+1,j,m),cc=vert(i+1,j+1,m),d=vert(i,j+1,m);
      idx[m].push(a,b,cc,a,cc,d);
    }
  });
  const g=new T.BufferGeometry();
  g.setAttribute('position',new T.Float32BufferAttribute(P,3));
  g.setAttribute('normal',new T.Float32BufferAttribute(N,3));
  g.setAttribute('color',new T.Float32BufferAttribute(Cc,3));
  g.setAttribute('uv',new T.Float32BufferAttribute(U,2));
  const all=[];let st=0;
  for(let m=0;m<3;m++){if(idx[m].length){g.addGroup(st,idx[m].length,m);for(const v of idx[m])all.push(v);st+=idx[m].length;}else g.addGroup(st,0,m);}
  g.setIndex(all);
  const floorMat=new T.MeshStandardMaterial({map:ARN.tex,roughness:.72,metalness:.05,side:T.DoubleSide,alphaTest:ARN.kind==='drop'?.5:0});
  const wtx=ARN.wtex||WALL.tex,wallMat=new T.MeshBasicMaterial({map:wtx||TX.wall,vertexColors:!wtx,transparent:true,depthWrite:false,side:T.DoubleSide});
  const ceilMat=new T.MeshBasicMaterial({map:TX.wall,color:0x6fa8ff,transparent:true,opacity:.16,depthWrite:false,side:T.DoubleSide});
  const mesh=new T.Mesh(g,[floorMat,wallMat,ceilMat]);
  arenaGroup.add(mesh);
}

/* carte ouverte (parkour) : un sol plat, bordé d'un liseré, sans murs ni plafond ni gradins */
function buildOpenFloor(){
  const fl=new T.Mesh(new T.PlaneGeometry(2*HX,2*HZ),new T.MeshStandardMaterial({map:ARN.tex,roughness:.72,metalness:.05,alphaTest:ARN.kind==='drop'?.5:0}));
  fl.rotation.x=-Math.PI/2;arenaGroup.add(fl);
  const gl=new T.MeshBasicMaterial({color:0x35ff9a});
  for(const sx of [-1,1]){const b=new T.Mesh(new T.BoxGeometry(.8,.5,2*HZ+.8),gl);b.position.set(sx*HX,.25,0);arenaGroup.add(b);}
  for(const sz of [-1,1]){const b=new T.Mesh(new T.BoxGeometry(2*HX+.8,.5,.8),gl);b.position.set(0,.25,sz*HZ);arenaGroup.add(b);}
}
/* ---------- stade : gradins, public, projecteurs ---------- */
const flashes=[];let roar=0;
function crowdTex(){
  const [c,x]=mkc(256,128);x.fillStyle='#0b0e1c';x.fillRect(0,0,256,128);
  const cols=['#2f8bff','#ff8a2a','#ffffff','#e94560','#9fb4d8','#3b4466','#f5c542','#46d39a'];
  for(let i=0;i<900;i++){x.fillStyle=cols[(rnd()*cols.length)|0];x.globalAlpha=.35+rnd()*.6;const s=2+rnd()*3;x.fillRect(rnd()*256,rnd()*128,s,s*1.4);}
  x.globalAlpha=1;
  for(let i=0;i<10;i++){x.fillStyle='rgba(0,0,0,.45)';x.fillRect(0,i*13+9,256,3);}
  return ctex(c,true);
}
function buildStadium(){
  const ground=new T.Mesh(new T.PlaneGeometry(700,700),new T.MeshBasicMaterial({color:0x070a16}));
  ground.rotation.x=-Math.PI/2;ground.position.y=-.12;arenaGroup.add(ground);
  const ct=crowdTex();
  const mat=new T.MeshBasicMaterial({map:ct,color:0x8a93b8});
  const top=new T.MeshBasicMaterial({color:0x10142a});
  const side=(len,axis,sgn,start)=>{
    const rows=9;
    for(let i=0;i<rows;i++){
      const d=start+i*2.3,h=3+i*2.1,dep=2.3;
      const geo=new T.BoxGeometry(axis==='x'?dep:len,h,axis==='x'?len:dep);
      const uv=geo.attributes.uv;for(let k=0;k<uv.count;k++)uv.setXY(k,uv.getX(k)*len/9,uv.getY(k)*h/9);
      const m=new T.Mesh(geo,[mat,mat,top,top,mat,mat]);
      if(axis==='x')m.position.set(sgn*(d+dep/2),h/2-.12,0);else m.position.set(0,h/2-.12,sgn*(d+dep/2));
      arenaGroup.add(m);
      // spectateurs qui prennent des photos
      for(let k=0;k<5;k++){
        const sp=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xffffff,transparent:true,blending:T.AdditiveBlending,depthWrite:false,opacity:0}));
        sp.scale.set(2.2,2.2,1);
        const u=(rnd()-.5)*len*.92;
        if(axis==='x')sp.position.set(sgn*(d+dep/2),h+1,u);else sp.position.set(u,h+1,sgn*(d+dep/2));
        sp.userData.t=0;sp.visible=false;arenaGroup.add(sp);flashes.push(sp);
      }
    }
  };
  const sideHex=()=>{ /* 6 tribunes parallèles aux 6 murs de l'hexagone */
    for(let e=0;e<6;e++){
      const ph=(30+60*e)*Math.PI/180,nx=Math.cos(ph),nz=Math.sin(ph),tx=-nz,tz=nx;
      for(let i=0;i<9;i++){
        const d=HZ+5+i*2.3,h=3+i*2.1,dep=2.3,len=(d+dep)*1.1547+3,cd=d+dep/2;
        const geo=new T.BoxGeometry(len,h,dep);
        const uv=geo.attributes.uv;for(let k=0;k<uv.count;k++)uv.setXY(k,uv.getX(k)*len/9,uv.getY(k)*h/9);
        const m=new T.Mesh(geo,[mat,mat,top,top,mat,mat]);
        m.position.set(nx*cd,h/2-.12,nz*cd);m.rotation.y=Math.PI/2-ph;arenaGroup.add(m);
        for(let k=0;k<5;k++){
          const sp=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xffffff,transparent:true,blending:T.AdditiveBlending,depthWrite:false,opacity:0}));
          sp.scale.set(2.2,2.2,1);
          const u=(rnd()-.5)*len*.92;
          sp.position.set(nx*cd+tx*u,h+1,nz*cd+tz*u);
          sp.userData.t=0;sp.visible=false;arenaGroup.add(sp);flashes.push(sp);
        }
      }
    }
  };
  if(DROP.on)sideHex();else{
  side(2*(HZ+14),'x',1,HX+5);side(2*(HZ+14),'x',-1,HX+5);
  side(2*(HX+14),'z',1,HZ+GD+4);side(2*(HX+14),'z',-1,HZ+GD+4);}
  // projecteurs d'angle
  const lampM=new T.MeshBasicMaterial({color:0xfff6dd});
  const LPS=[];
  if(DROP.on){for(let i=0;i<6;i++)LPS.push([Math.cos(i*Math.PI/3)*(HX+32),Math.sin(i*Math.PI/3)*(HX+32)]);}
  else for(const sx of [-1,1])for(const sz of [-1,1])LPS.push([sx*(HX+30),sz*(HZ+34)]);
  for(const [px,pz] of LPS){
    const pole=new T.Mesh(new T.BoxGeometry(2,52,2),new T.MeshBasicMaterial({color:0x151a30}));pole.position.set(px,26,pz);arenaGroup.add(pole);
    const lamp=new T.Mesh(new T.BoxGeometry(14,5,2),lampM);lamp.position.set(px,53,pz);lamp.lookAt(0,10,0);arenaGroup.add(lamp);
    const gs=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xcfe4ff,transparent:true,blending:T.AdditiveBlending,depthWrite:false,fog:false}));
    gs.scale.set(46,46,1);gs.position.set(px,53,pz);arenaGroup.add(gs);
  }
  // anneau lumineux au-dessus des gradins
  if(DROP.on){
    for(let e=0;e<6;e++){
      const ph=(30+60*e)*Math.PI/180,nx=Math.cos(ph),nz=Math.sin(ph),d=HZ+4.5;
      const b=new T.Mesh(new T.BoxGeometry(d*1.1547+1,.6,.6),new T.MeshBasicMaterial({color:nz<0?0x2f8bff:0xff8a2a}));
      b.position.set(nx*d,12,nz*d);b.rotation.y=Math.PI/2-ph;arenaGroup.add(b);
    }
  }else for(const sx of [-1,1]){
    const b=new T.Mesh(new T.BoxGeometry(.6,.6,2*(HZ+14)),new T.MeshBasicMaterial({color:sx<0?0x2f8bff:0xff8a2a}));b.position.set(sx*(HX+4.5),12,0);arenaGroup.add(b);
  }
}
function crowdRoar(){roar=5;}
function updateStadium(dt){
  roar=Math.max(0,roar-dt);
  const rate=(roar>0?60:5)*dt;let n=Math.floor(rate)+(rnd()<rate%1?1:0);
  while(n-->0){const s=flashes[(rnd()*flashes.length)|0];if(s&&s.userData.t<=0)s.userData.t=.12+rnd()*.12;}
  for(const s of flashes){if(s.userData.t>0){s.userData.t-=dt;s.material.opacity=.9;s.visible=s.userData.t>0;}}
}

/* ---------- cages ---------- */
function labelTex(text,css){
  const [c,x]=mkc(256,64);x.font='bold 34px system-ui,Arial';x.textAlign='center';x.textBaseline='middle';
  x.fillStyle='rgba(5,8,20,.55)';x.fillRect(0,10,256,44);x.lineWidth=5;x.strokeStyle='rgba(0,0,0,.85)';x.strokeText(text,128,34);x.fillStyle=css;x.fillText(text,128,34);
  return new T.CanvasTexture(c);
}
function makeLabel(text,css){
  const sp=new T.Sprite(new T.SpriteMaterial({map:labelTex(text,css),transparent:true,depthTest:false,depthWrite:false}));
  sp.scale.set(5.5,1.4,1);sp.renderOrder=10;return sp;
}
function buildGoal(s,team){
  const g=new T.Group(),col=TEAM[team];
  const mk=(rx,ry)=>{const t=TX.net.clone();t.needsUpdate=true;t.wrapS=t.wrapT=T.RepeatWrapping;t.repeat.set(rx,ry);return new T.MeshBasicMaterial({map:t,color:col,transparent:true,opacity:.85,side:T.DoubleSide,depthWrite:false});};
  const side=mk(GD/2,GH/2),top=mk(GW,GD/2),back=mk(GW,GH/2),hide=new T.MeshBasicMaterial({visible:false});
  const box=new T.Mesh(new T.BoxGeometry(2*GW,GH,GD),[side,side,top,hide,s>0?back:hide,s>0?hide:back]);
  box.position.set(0,GH/2,s*(HZ+GD/2));g.add(box);
  const fl=new T.Mesh(new T.PlaneGeometry(2*GW,GD+R),new T.MeshStandardMaterial({color:0x101a30,emissive:col,emissiveIntensity:.22,roughness:.6}));
  fl.rotation.x=-Math.PI/2;fl.position.set(0,.03,s*(HZ+(GD-R)/2));g.add(fl);
  const gl=new T.MeshBasicMaterial({color:col});
  for(const sx of [-1,1]){const p=new T.Mesh(new T.BoxGeometry(.8,GH+.8,.8),gl);p.position.set(sx*GW,GH/2,s*HZ);g.add(p);}
  const cb=new T.Mesh(new T.BoxGeometry(2*GW+.8,.8,.8),gl);cb.position.set(0,GH,s*HZ);g.add(cb);
  // flancs de la découpe dans les rampes
  const N=10,pts=[];for(let k=0;k<=N;k++){const th=k/N*Math.PI/2;pts.push([HZ-R+R*Math.sin(th),R-R*Math.cos(th)]);}
  for(const sx of [-1,1]){
    const pos=[];
    for(let k=0;k<N;k++){
      pos.push(sx*GW,0,s*HZ, sx*GW,pts[k][1],s*pts[k][0], sx*GW,pts[k+1][1],s*pts[k+1][0]);
    }
    const cg=new T.BufferGeometry();cg.setAttribute('position',new T.Float32BufferAttribute(pos,3));cg.computeVertexNormals();
    g.add(new T.Mesh(cg,new T.MeshStandardMaterial({color:0x131b34,emissive:col,emissiveIntensity:.35,side:T.DoubleSide,roughness:.5})));
  }
  const sign=new T.Mesh(new T.PlaneGeometry(18,4.2),new T.MeshBasicMaterial({map:labelTex(TNAME[team],TCSS[team]),transparent:true}));
  sign.position.set(0,15,s*(HZ-.5));sign.rotation.y=s>0?Math.PI:0;sign.scale.set(1.4,1.4,1);g.add(sign);
  arenaGroup.add(g);
}

/* ---------- boost pads ---------- */
const arenaGroup=new T.Group();scene.add(arenaGroup);
const pads=[];
function buildPads(){
  const sxs=HX/40,szs=HZ/60;
  const Q=[[0,50],[12,46],[22,40],[8,34],[18,26],[0,24],[27,12],[12,10],[0,0]];
  const add=(x,z,big)=>{
    if(DROP.on){const v={x:x,z:z};hexFit(v,3);x=v.x;z=v.z;}
    const g=new T.Group(),sz=big?7:3.6;
    const d=new T.Mesh(new T.PlaneGeometry(sz,sz),new T.MeshBasicMaterial({map:TX.pad,transparent:true,depthWrite:false,blending:T.AdditiveBlending}));
    d.rotation.x=-Math.PI/2;d.position.y=.06;g.add(d);
    let orb=null;
    if(big){orb=new T.Mesh(new T.IcosahedronGeometry(1.2,0),new T.MeshBasicMaterial({color:0xffc266}));orb.position.y=2.2;g.add(orb);
      const gs=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xff9a30,transparent:true,blending:T.AdditiveBlending,depthWrite:false}));gs.scale.set(6,6,1);orb.add(gs);}
    g.position.set(x,0,z);arenaGroup.add(g);
    pads.push({x:x,z:z,big:big,active:true,t:0,g:g,orb:orb,d:d});
  };
  /* vrai placement RL : 28 petites pastilles + 6 grosses (coordonnées uu du vrai jeu) */
  const SM=[[0,4240],[1792,4184],[-1792,4184],[940,3308],[-940,3308],[0,2816],[3584,2484],[-3584,2484],[1788,2300],[-1788,2300],[2048,1036],[-2048,1036],[0,1024]];
  SM.forEach(q=>[1,-1].forEach(k=>add(q[0]/4096*HX,k*q[1]/5120*HZ,false)));
  [[1024,0],[-1024,0]].forEach(q=>add(q[0]/4096*HX,0,false));
  [[3584,0],[-3584,0],[3072,4096],[-3072,4096],[3072,-4096],[-3072,-4096]].forEach(q=>add(q[0]/4096*HX,q[1]/5120*HZ,true));
}
function resetPads(){pads.forEach(p=>{p.active=true;p.t=0;p.g.visible=true;p.d.material.opacity=1;if(p.orb)p.orb.visible=true;});}
function nearestPad(pos,maxD){
  let best=null,bd=maxD*maxD;
  for(const p of pads){if(!p.active)continue;const dx=p.x-pos.x,dz=p.z-pos.z;const d=(dx*dx+dz*dz)/(p.big?2.2:1);if(d<bd){bd=d;best=p;}}
  return best;
}

/* ---------- Dropshot : dalles hexagonales ---------- */
function dropBuildTiles(){
  DROP.tiles=[];const rad=DROP.rad,dx=rad*1.7320508,dz=rad*1.5;
  for(let r=0;r<13;r++){ /* plateau hexagonal de 127 dalles : 7-8-9-10-11-12-13-12-…-7 ; la rangée centrale (z=0) est neutre */
    const z=(r-6)*dz,n=13-Math.abs(r-6);
    for(let k=0;k<n;k++){const x=(k-(n-1)/2)*dx;DROP.tiles.push({x:x,z:z,team:r<6?0:r>6?1:-1,st:0});}
  }
  DROP.tiles.forEach(t=>{t.nb=DROP.tiles.filter(u=>u!==t&&Math.hypot(u.x-t.x,u.z-t.z)<dx*1.05);});
  DROP.charge=0;DROP.team=-1;
}
function dropDrawTile(t){
  const x=DROP.ctx;if(!x)return;
  const s=DROP.s,cx=DROP.X(t.x),cz=DROP.Z(t.z),r=DROP.rad*s*.95;
  const col=t.team===0?'47,139,255':t.team===1?'255,138,42':'190,160,255';
  x.beginPath();
  for(let k=0;k<6;k++){const a=-Math.PI/2+k*Math.PI/3;const px=cx+Math.cos(a)*r,pz=cz+Math.sin(a)*r;k?x.lineTo(px,pz):x.moveTo(px,pz);}
  x.closePath();
  if(t.st>=2){ /* dalle détruite : on la rend totalement transparente (le sol est en alphaTest), on ne voit plus rien */
    x.save();x.globalCompositeOperation='destination-out';x.fillStyle='#000';x.fill();x.lineWidth=10;x.strokeStyle='#000';x.stroke();x.restore();return;
  }
  x.save();x.lineWidth=9;x.strokeStyle='#0b1426';x.stroke();x.restore(); /* efface l'ancien contour (trou/fissure) */
  x.fillStyle='#0b1426';x.fill();
  x.fillStyle='rgba('+col+','+(t.st?.4:.2)+')';x.fill();
  x.lineWidth=4;x.strokeStyle='rgba('+col+',.95)';x.stroke();
  if(t.st===1)dropCracks(x,cx,cz,r,col,Math.round(t.x*131+t.z*977));
}
/* motif de fissures détaillé : ~10 fissures principales en zigzag depuis le point d'impact, ramifications récursives (3 niveaux),
   fissures de contrainte en arcs concentriques, éclats de dalle ombrés entre les fissures et poussière autour de l'impact.
   Toujours le même motif pour une dalle donnée (graine = position). La dalle fissurée reste visible et pleine ; seule la dalle détruite (invisible) laisse tomber la balle (voir dropHole). */
function dropCracks(x,cx,cz,r,col,seed){
  let q=(seed*2654435761)>>>0;const R=()=>{q=(Math.imul(q,1664525)+1013904223)>>>0;return q/4294967296;};
  const lines=[],rays=[],n=9+((R()*4)|0),a0=R()*Math.PI*2;
  const grow=(px,pz,a,len,steps,depth,w,keep)=>{
    const pts=[[px,pz]];
    for(let j=1;j<=steps;j++){
      a+=(R()-.5)*(.75-depth*.12);
      const d=len/steps*(.65+R()*.7);px+=Math.cos(a)*d;pz+=Math.sin(a)*d;pts.push([px,pz]);
      if(depth<3&&j<steps&&R()<.62-depth*.1){
        const side=R()<.5?-1:1;
        grow(px,pz,a+side*(.4+R()*.8),len*(.28+R()*.3)*(1-j/steps*.45),3+((R()*3)|0),depth+1,w*.62);
        if(depth<2&&R()<.25)grow(px,pz,a-side*(.5+R()*.7),len*(.18+R()*.2),3,depth+1,w*.5); /* fourche */
      }
    }
    lines.push({p:pts,w:w});
    if(keep)rays.push({a:a0,p:pts});
  };
  for(let k=0;k<n;k++){
    const ang=a0+k*Math.PI*2/n+(R()-.5)*.55;
    grow(cx+(R()-.5)*r*.06,cz+(R()-.5)*r*.06,ang,r*(.85+R()*.4),7+((R()*3)|0),0,1,true);
  }
  /* fissures de contrainte : arcs brisés qui relient deux fissures voisines autour de l'impact */
  const rings=2+((R()*2)|0);
  for(let m=0;m<rings;m++){
    const rr=r*(.16+m*.14+R()*.05);
    for(let k=0;k<n;k++){
      if(R()<.4)continue;
      const aS=Math.atan2(rays[k].p[1][1]-cz,rays[k].p[1][0]-cx),span=Math.PI*2/n*(.5+R()*.6),pts=[];
      const seg=4+((R()*2)|0);
      for(let j=0;j<=seg;j++){const aa=aS+span*j/seg+(R()-.5)*.08,rj=rr*(.88+R()*.24);pts.push([cx+Math.cos(aa)*rj,cz+Math.sin(aa)*rj]);}
      lines.push({p:pts,w:.5});
    }
  }
  x.save();
  x.beginPath();for(let k=0;k<6;k++){const a=-Math.PI/2+k*Math.PI/3;const hx=cx+Math.cos(a)*r*.98,hz=cz+Math.sin(a)*r*.98;k?x.lineTo(hx,hz):x.moveTo(hx,hz);}x.closePath();x.clip();
  /* éclats : secteurs entre fissures voisines, légèrement plus clairs/sombres */
  for(let k=0;k<n;k++){
    const A=rays[k].p,B=rays[(k+1)%n].p,e=Math.min(4,A.length-1,B.length-1);
    x.beginPath();x.moveTo(cx,cz);for(let j=1;j<=e;j++)x.lineTo(A[j][0],A[j][1]);for(let j=e;j>=1;j--)x.lineTo(B[j][0],B[j][1]);x.closePath();
    x.fillStyle=k%2?'rgba(255,255,255,.07)':'rgba(0,0,0,.28)';x.fill();
  }
  x.lineCap='round';x.lineJoin='round';
  const pass=(wid,style)=>{x.strokeStyle=style;lines.forEach(l=>{x.lineWidth=Math.max(.5,wid*l.w);x.beginPath();l.p.forEach((u,i)=>i?x.lineTo(u[0],u[1]):x.moveTo(u[0],u[1]));x.stroke();});};
  pass(7,'rgba(0,4,14,.95)');                       /* fond sombre de la fissure */
  pass(4.2,'rgba('+col+',.55)');                    /* lueur de l'équipe */
  pass(1.8,'rgba(255,255,255,.95)');                /* cœur lumineux */
  /* poussière et éclats minuscules autour de l'impact */
  for(let k=0;k<26;k++){const aa=R()*Math.PI*2,dd=r*(.08+R()*.5);x.fillStyle=R()<.5?'rgba(255,255,255,.55)':'rgba(0,0,0,.6)';x.fillRect(cx+Math.cos(aa)*dd,cz+Math.sin(aa)*dd,1+R()*2,1+R()*2);}
  const g=x.createRadialGradient(cx,cz,0,cx,cz,r*.3);g.addColorStop(0,'rgba(255,255,255,.85)');g.addColorStop(.5,'rgba('+col+',.35)');g.addColorStop(1,'rgba(255,255,255,0)');
  x.fillStyle=g;x.beginPath();x.arc(cx,cz,r*.3,0,Math.PI*2);x.fill();  /* point d'impact */
  x.restore();
}
function dropDamage(t){
  if(t.st>=2)return;t.st++;dropDrawTile(t);if(DROP.tex)DROP.tex.needsUpdate=true;
  burst(tA.set(t.x,.6,t.z),t.team===0?0x2f8bff:t.team===1?0xff8a2a:0xb49cff,t.st>=2?26:10,t.st>=2?16:9,.7);
  if(t.st>=2)SFX.demo();
}
function dropImpact(x,z){
  if(!DROP.on||DROP.charge<=0||DROP.team<0)return;
  let best=null,bd=1e9;
  for(const t of DROP.tiles){const d=(t.x-x)*(t.x-x)+(t.z-z)*(t.z-z);if(d<bd){bd=d;best=t;}}
  if(!best||bd>DROP.rad*DROP.rad*1.3)return;
  if(best.team===DROP.team)return;
  let set=[best];
  if(DROP.charge>=2)set=set.concat(best.nb);
  if(DROP.charge>=3){const s2=new Set(set);set.forEach(t=>t.nb.forEach(u=>s2.add(u)));set=Array.from(s2);}
  set.forEach(t=>{if(t.team!==DROP.team)dropDamage(t);});
  if(player&&DROP.team===player.team)popup(best.st>=2?'DALLE DÉTRUITE':'DALLE FISSURÉE',0,'#ffd166');
  DROP.charge=0;
}
function dropRepair(all){
  DROP.tiles.forEach(t=>{if(all)t.st=0;else if(t.st>=2)t.st=1;dropDrawTile(t);});
  if(DROP.tex)DROP.tex.needsUpdate=true;
}
function dropTileAt(x,z){
  let best=null,bd=1e9;for(const t of DROP.tiles){const d=(t.x-x)*(t.x-x)+(t.z-z)*(t.z-z);if(d<bd){bd=d;best=t;}}return best;
}

/* ---------- Hoops : panneaux et paniers ---------- */
function buildHoops(){
  HOOP.on=true;
  for(const s of [-1,1]){
    const col=s<0?TEAM[0]:TEAM[1],g=new T.Group();
    const bb=new T.Mesh(new T.BoxGeometry(HOOP.bw*2,HOOP.bh*2,1.2),new T.MeshStandardMaterial({color:0xdfeaff,metalness:.3,roughness:.2,transparent:true,opacity:.55}));
    bb.position.set(0,HOOP.by,s*(HZ-.6));g.add(bb);
    const fm=new T.MeshBasicMaterial({color:col});
    const fr=(x,y,w,h)=>{const m=new T.Mesh(new T.BoxGeometry(w,h,.5),fm);m.position.set(x,HOOP.by+y,s*(HZ-1.3));g.add(m);};
    fr(0,HOOP.bh,HOOP.bw*2+.6,.5);fr(0,-HOOP.bh,HOOP.bw*2+.6,.5);fr(HOOP.bw,0,.5,HOOP.bh*2);fr(-HOOP.bw,0,.5,HOOP.bh*2);
    const ring=new T.Mesh(new T.TorusGeometry(HOOP.Rc,HOOP.rt,10,36),new T.MeshStandardMaterial({color:0xff7a1a,emissive:0xff4a00,emissiveIntensity:.6,metalness:.4,roughness:.35}));
    ring.rotation.x=Math.PI/2;ring.position.set(0,HOOP.ry,s*(HZ-HOOP.rz));g.add(ring);
    const net=new T.Mesh(new T.CylinderGeometry(HOOP.Rc,HOOP.Rc*.55,4,20,5,true),new T.MeshBasicMaterial({color:0xffffff,transparent:true,opacity:.7,wireframe:true}));
    net.position.set(0,HOOP.ry-2,s*(HZ-HOOP.rz));g.add(net);
    const arm=new T.Mesh(new T.BoxGeometry(.8,.6,HOOP.rz-HOOP.Rc-.9),new T.MeshStandardMaterial({color:0x8a93a6,metalness:.6,roughness:.4}));
    arm.position.set(0,HOOP.ry,s*(HZ-1.2-(HOOP.rz-HOOP.Rc-.9)/2-.1));
    g.add(arm);
    const glow=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:col,transparent:true,blending:T.AdditiveBlending,depthWrite:false}));
    glow.scale.set(18,18,1);glow.position.set(0,HOOP.by,s*(HZ-2));g.add(glow);
    arenaGroup.add(g);
  }
}

/* ---------- thèmes + changement d'arène ---------- */
const THEMES={std:{bg:0x070b1a,fog:0x070b1a},snow:{bg:0x1a2a44,fog:0x30456a},hoops:{bg:0x140c08,fog:0x1c120b},drop:{bg:0x05060e,fog:0x05060e}};
function applyTheme(kind){
  const th=THEMES[kind]||THEMES.std;
  scene.background.set(th.bg);scene.fog.color.set(th.fog);
}
function setArena(kind,small){
  const a=Object.assign({},ARENAS[kind]||ARENAS.std,small&&(kind==='std'||kind==='snow')?{HX:40,HZ:60,H:24,GW:10,GH:9,GD:12}:{});
  if(CM.cur&&(kind==='std'||kind==='snow')){ /* carte perso : cages classiques retirées ou redimensionnées */
    if(CM.cur.defGoals===false){a.goals=false;a.GW=a.GH=a.GD=0;}
    else if(CM.cur.gs&&CM.cur.gs!==1){const k=CM.cur.gs;a.GW=a.GW*k;a.GH=a.GH*k;a.GD=a.GD*Math.min(k,1.3);}
  }
  ARN.kind=ARENAS[kind]?kind:'std';
  HX=a.HX;HZ=a.HZ;H=a.H;HY=H/2;GW=a.GW;GH=a.GH;GD=a.GD;HASGOAL=a.goals;GCZ=HZ+(GD-R)/2;GHZ=(GD+R)/2;
  HOOP.on=false;DROP.on=false;
  OPEN=!!(CM.cur&&CM.cur.mode==='parkour'&&(kind==='std'||kind==='snow'));
  recomputeCO();
  while(arenaGroup.children.length){const o=arenaGroup.children[0];arenaGroup.remove(o);o.traverse(m=>{if(m.geometry&&!m.isSprite)m.geometry.dispose();});}
  if(ARN.tex&&ARN.tex.dispose)ARN.tex.dispose();
  if(ARN.wtex&&ARN.wtex.dispose)ARN.wtex.dispose();ARN.wtex=null;
  flashes.length=0;pads.length=0;
  if(ARN.kind==='drop'){DROP.on=true;dropBuildTiles();}
  ARN.tex=(CM.cur&&CM.cur.texF)?cmTex(CM.cur.texF,1):floorTex(ARN.kind);
  if(CM.cur&&CM.cur.texW)ARN.wtex=cmTex(CM.cur.texW,CM.cur.texR);
  if(OPEN)buildOpenFloor();else{buildArena();buildStadium();}
  if(HASGOAL){buildGoal(-1,0);buildGoal(1,1);}
  if(!(CM.cur&&!CM.cur.defPads))buildPads();
  if(ARN.kind==='hoops')buildHoops();
  if(CM.cur)cmBuild();
  applyTheme(ARN.kind);
}
setArena('std');

/* ---------- particules ---------- */
const NP=3200,pPos=new Float32Array(NP*3),pCol=new Float32Array(NP*3),pVel=new Float32Array(NP*3),pBase=new Float32Array(NP*3),pLife=new Float32Array(NP),pMax=new Float32Array(NP),pGr=new Float32Array(NP);
let pHead=0;
for(let i=0;i<NP;i++)pPos[i*3+1]=-999;
const pGeo=new T.BufferGeometry();
pGeo.setAttribute('position',new T.BufferAttribute(pPos,3));pGeo.setAttribute('color',new T.BufferAttribute(pCol,3));
const points=new T.Points(pGeo,new T.PointsMaterial({size:1.5,map:TX.glow,vertexColors:true,transparent:true,depthWrite:false,blending:T.AdditiveBlending,sizeAttenuation:true,fog:false}));
points.frustumCulled=false;scene.add(points);
function emit(x,y,z,vx,vy,vz,r,g,b,life,gr){
  const i=pHead;pHead=(pHead+1)%NP;const k=i*3;
  pPos[k]=x;pPos[k+1]=y;pPos[k+2]=z;pVel[k]=vx;pVel[k+1]=vy;pVel[k+2]=vz;pBase[k]=r;pBase[k+1]=g;pBase[k+2]=b;pLife[i]=life;pMax[i]=life;pGr[i]=(gr===undefined?8:gr);
}
function burst(p,col,n,spd,life,gr){
  const r=((col>>16)&255)/255,g=((col>>8)&255)/255,b=(col&255)/255;
  for(let i=0;i<n;i++){
    const a=rnd()*Math.PI*2,u=rnd()*2-1,s=Math.sqrt(1-u*u),v=spd*(.35+rnd()*.65);
    emit(p.x,p.y,p.z,Math.cos(a)*s*v,u*v,Math.sin(a)*s*v,r,g,b,life*(.6+rnd()*.6),gr);
  }
}
function updParticles(dt){
  for(let i=0;i<NP;i++){
    if(pLife[i]<=0)continue;
    pLife[i]-=dt;const k=i*3;
    if(pLife[i]<=0){pPos[k+1]=-999;pCol[k]=pCol[k+1]=pCol[k+2]=0;continue;}
    pVel[k+1]-=pGr[i]*dt;pPos[k]+=pVel[k]*dt;pPos[k+1]+=pVel[k+1]*dt;pPos[k+2]+=pVel[k+2]*dt;
    const f=pLife[i]/pMax[i];pCol[k]=pBase[k]*f;pCol[k+1]=pBase[k+1]*f;pCol[k+2]=pBase[k+2]*f;
  }
  pGeo.attributes.position.needsUpdate=true;pGeo.attributes.color.needsUpdate=true;
}

/* ---------- traces de dérapage (instances au sol) ---------- */
const NSK=500,skid=new T.InstancedMesh(new T.PlaneGeometry(.38,1.1),new T.MeshBasicMaterial({color:0x000000,transparent:true,opacity:.38,depthWrite:false,polygonOffset:true,polygonOffsetFactor:-2,polygonOffsetUnits:-2}),NSK);
skid.frustumCulled=false;scene.add(skid);
{const z=new T.Matrix4().makeScale(0,0,0);for(let i=0;i<NSK;i++)skid.setMatrixAt(i,z);skid.instanceMatrix.needsUpdate=true;}
let skHead=0;const skM=new T.Matrix4(),skQ=new T.Quaternion(),skS=new T.Vector3(1,1,1),skP=new T.Vector3(),skE=new T.Euler(-Math.PI/2,0,0);
function addSkid(x,z,yaw){
  skE.set(-Math.PI/2,yaw,0,'YXZ');skQ.setFromEuler(skE);skP.set(x,.045,z);skM.compose(skP,skQ,skS);
  skid.setMatrixAt(skHead,skM);skHead=(skHead+1)%NSK;skid.instanceMatrix.needsUpdate=true;
}
function clearSkids(){const z=new T.Matrix4().makeScale(0,0,0);for(let i=0;i<NSK;i++)skid.setMatrixAt(i,z);skid.instanceMatrix.needsUpdate=true;}

/* ---------- caméra : tremblements ---------- */
let shake=0;
function addShake(v){shake=Math.min(1.2,Math.max(shake,v));}

/* ---------- balle ---------- */
const ball={pos:V3(0,BR,0),vel:V3(),alive:true,mesh:null,last:null,prev:null,lastT:0,prevT:0,shotTeam:-1,halo:null,frozen:0,held:null,rest:.62,py:BR,pm:null,sphere:null,puck:false};
ball.mesh=new T.Mesh(new T.SphereGeometry(BR,36,24),new T.MeshStandardMaterial({map:TX.ball,emissiveMap:TX.ball,emissive:new T.Color(0x3a5a88),roughness:.32,metalness:.35}));
ball.halo=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xffffff,transparent:true,blending:T.AdditiveBlending,depthWrite:false,opacity:.5}));
ball.halo.scale.set(13,13,1);ball.mesh.add(ball.halo);
scene.add(ball.mesh);
const ballShadow=new T.Mesh(new T.CircleGeometry(1,24),new T.MeshBasicMaterial({color:0,transparent:true,opacity:.4,depthWrite:false}));
ballShadow.rotation.x=-Math.PI/2;scene.add(ballShadow);

/* =====================================================================
   VOITURES (modèles)
   ===================================================================== */
/* =====================================================================
   VOITURES : rendu paramétrique (100 carrosseries + cosmétiques)
   ===================================================================== */
const GC=new Map();
function cg(key,fn){let g=GC.get(key);if(!g){g=fn();GC.set(key,g);}return g;}
const MC=new Map();
function sm(color,metal,rough,emis,eInt){
  const key=[color,metal,rough,emis||0,eInt||0].join('|');
  let m=MC.get(key);
  if(!m){m=new T.MeshStandardMaterial({color:color,metalness:metal,roughness:rough,emissive:emis||0,emissiveIntensity:eInt||0});MC.set(key,m);}
  return m;
}
function bm(color,op,add){
  const key='b|'+[color,op===undefined?1:op,add?1:0].join('|');
  let m=MC.get(key);
  if(!m){const tr=op!==undefined&&op<1;m=new T.MeshBasicMaterial({color:color,transparent:tr||!!add,opacity:op===undefined?1:op,blending:add?T.AdditiveBlending:T.NormalBlending,depthWrite:!(tr||add)});MC.set(key,m);}
  return m;
}
const G3={
  box:(w,h,d)=>cg('b'+w+','+h+','+d,()=>new T.BoxGeometry(w,h,d)),
  cyl:(a,b,h,s)=>cg('c'+[a,b,h,s].join(','),()=>new T.CylinderGeometry(a,b,h,s||14)),
  sph:(r)=>cg('s'+r,()=>new T.SphereGeometry(r,14,10)),
  cone:(r,h,s)=>cg('k'+[r,h,s].join(','),()=>new T.ConeGeometry(r,h,s||12)),
  tor:(r,t)=>cg('t'+r+','+t,()=>new T.TorusGeometry(r,t,8,22))
};
function pt(geo,mat,x,y,z,rx,ry,rz){
  const m=new T.Mesh(geo,mat);m.position.set(x||0,y||0,z||0);if(rx||ry||rz)m.rotation.set(rx||0,ry||0,rz||0);return m;
}
function shapeGeo(key,build,depth){
  return cg('sh'+key,()=>{const s=new T.Shape();build(s);const g=new T.ExtrudeGeometry(s,{depth:depth,bevelEnabled:false,curveSegments:8});g.translate(0,0,-depth/2);return g;});
}
function shapeOf(pts){const s=new T.Shape();s.moveTo(pts[0][0],pts[0][1]);for(let i=1;i<pts.length;i++)s.lineTo(pts[i][0],pts[i][1]);s.closePath();return s;}
function extr(pts,w,bev){
  const g=new T.ExtrudeGeometry(shapeOf(pts),{depth:w-2*bev,bevelEnabled:true,bevelThickness:bev,bevelSize:bev,bevelSegments:2,curveSegments:2});
  g.translate(0,0,-(w-2*bev)/2);g.rotateY(Math.PI/2);return g;
}

/* ---------- roues ---------- */
function rimMat(cm,acc){
  switch(cm){
    case 'wht':return sm(0xf4f7ff,.4,.3);
    case 'gold':return sm(0xffc933,.9,.25);
    case 'chr':return sm(0xe8eef8,1,.08);
    case 'blk':return sm(0x14161c,.4,.4);
    case 'neon':return bm(acc);
    default:return sm(acc,.6,.3,acc,.25);
  }
}
function wheelGroup(wd,wr,acc){
  const g=new T.Group(),tw=.5*wd.tw,rr=wr*wd.rs,rm=rimMat(wd.cm,acc),wdh=tw*1.04;
  g.add(new T.Mesh(cg('tire'+wr+'|'+tw,()=>{const x=new T.CylinderGeometry(wr,wr,tw,22);x.rotateZ(Math.PI/2);return x;}),sm(0x14161c,.2,.7)));
  const cylX=(key,r,w,seg)=>cg(key+r+'|'+w,()=>{const x=new T.CylinderGeometry(r,r,w,seg);x.rotateZ(Math.PI/2);return x;});
  const spokes=(n,w,len,twist)=>{
    for(let i=0;i<n;i++){const piv=new T.Group();piv.rotation.x=i*Math.PI*2/n;const m=pt(G3.box(wdh,len,w),rm,0,len/2,0);if(twist)m.rotation.y=twist;piv.add(m);g.add(piv);}
  };
  const ring=(r,t)=>g.add(new T.Mesh(cg('ring'+r+'|'+t,()=>{const x=new T.TorusGeometry(r,t,8,28);x.rotateY(Math.PI/2);return x;}),rm));
  const hub=r=>g.add(new T.Mesh(cylX('hub',r,wdh*1.04,12),sm(0x20242e,.6,.3)));
  if(wd.st!=='disc'&&wd.st!=='hex')g.add(new T.Mesh(cylX('back',rr*.98,tw*1.02,16),sm(0x0a0c10,.2,.8)));
  switch(wd.st){
    case 'disc':g.add(new T.Mesh(cylX('disc',rr,wdh,24),rm));hub(rr*.3);break;
    case 'hex':g.add(new T.Mesh(cylX('hex',rr,wdh,6),rm));hub(rr*.3);break;
    case 'mesh':ring(rr*.92,.05);ring(rr*.45,.04);spokes(wd.n,.05,rr);hub(rr*.2);break;
    case 'ring':ring(rr*.95,.09);spokes(4,.07,rr*.95);hub(rr*.25);break;
    case 'star':spokes(wd.n,.14,rr);hub(rr*.25);break;
    case 'turbine':spokes(wd.n,.11,rr,.7);hub(rr*.22);break;
    case 'split':
      for(let i=0;i<wd.n;i++)for(const sg of [-1,1]){const piv=new T.Group();piv.rotation.x=i*Math.PI*2/wd.n+sg*.16;piv.add(pt(G3.box(wdh,rr,.06),rm,0,rr/2,0));g.add(piv);}
      hub(rr*.22);break;
    default:spokes(wd.n,.1,rr);hub(rr*.22);
  }
  return g;
}

/* ---------- toppers ---------- */
const GOLD=()=>sm(0xffc933,.9,.25);
function chkMat(){
  return cg('chkMat',()=>{const [c,x]=mkc(64,64);for(let i=0;i<8;i++)for(let j=0;j<8;j++){x.fillStyle=(i+j)%2?'#111':'#fff';x.fillRect(i*8,j*8,8,8);}
    return new T.MeshBasicMaterial({map:new T.CanvasTexture(c),side:T.DoubleSide});});
}
const TOPFN=[null,
  (a,p)=>{const g=new T.Group(),k=sm(0x151515,.3,.4);g.add(pt(G3.cyl(.62,.62,.06,18),k,0,.03,0),pt(G3.cyl(.38,.38,.7,18),k,0,.4,0),pt(G3.cyl(.39,.39,.14,18),sm(a,.5,.3),0,.15,0));return g;},
  (a,p)=>{const g=new T.Group(),m=GOLD();g.add(pt(G3.cyl(.5,.5,.2,16),m,0,.1,0));for(let i=0;i<6;i++){const an=i*Math.PI/3;g.add(pt(G3.cone(.1,.34,6),m,Math.cos(an)*.46,.35,Math.sin(an)*.46));g.add(pt(G3.sph(.06),sm(0xff3b3b,.4,.3),Math.cos(an)*.46,.55,Math.sin(an)*.46));}return g;},
  (a,p)=>{const g=new T.Group(),pr=new T.Group();g.add(pt(G3.sph(.42),sm(p,.4,.4),0,.1,0));g.add(pt(G3.box(.5,.05,.4),sm(p,.4,.4),0,.12,-.45));pr.position.y=.62;g.add(pt(G3.cyl(.04,.04,.2,8),sm(0xcccccc,.6,.3),0,.5,0));pr.add(pt(G3.box(.9,.03,.12),sm(a,.5,.3),0,0,0),pt(G3.box(.12,.03,.9),sm(a,.5,.3),0,0,0));g.add(pr);g.userData.spin=[{o:pr,ax:'y',v:18}];return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cone(.4,.9,16),sm(0xff6a10,.2,.5),0,.45,0),pt(G3.cyl(.27,.31,.1,16),sm(0xffffff,.1,.5),0,.35,0),pt(G3.cyl(.16,.2,.1,16),sm(0xffffff,.1,.5),0,.62,0),pt(G3.box(.85,.05,.85),sm(0x222222,.2,.6),0,.02,0));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cone(.35,.95,14),sm(a,.4,.4),0,.47,0),pt(G3.sph(.1),bm(0xffffff),0,.97,0),pt(G3.tor(.3,.04),sm(0xffffff,.2,.5),0,.2,0,Math.PI/2,0,0));return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0x2a2a7a,.3,.5);g.add(pt(G3.cyl(.72,.72,.04,18),m,0,.03,0));const c=pt(G3.cone(.4,1.1,16),m,.08,.6,0,0,0,.18);g.add(c);g.add(pt(G3.sph(.07),bm(a),.18,.8,-.4));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.sph(.45),sm(0x9aa3b5,.8,.3),0,.1,0),pt(G3.cone(.11,.55,8),sm(0xf5ecd0,.1,.6),.5,.35,0,0,0,-.55),pt(G3.cone(.11,.55,8),sm(0xf5ecd0,.1,.6),-.5,.35,0,0,0,.55));g.add(pt(G3.cyl(.46,.46,.08,16),sm(0x7a8294,.7,.4),0,.12,0));return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0x7a4a1a,.1,.7);g.add(pt(G3.cyl(.8,.8,.04,20),m,0,.03,0),pt(G3.cyl(.38,.46,.36,16),m,0,.22,0),pt(G3.cyl(.47,.47,.07,16),sm(a,.4,.4),0,.1,0));return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0x3a3a44,.2,.6);g.add(pt(G3.cyl(.62,.62,.04,18),m,0,.03,0),pt(G3.cyl(.34,.4,.32,16),m,0,.2,0),pt(G3.cyl(.41,.41,.08,16),sm(a,.4,.4),0,.1,0));return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0x151515,.2,.6);g.add(pt(G3.cone(.7,.4,3),m,0,.22,0,0,Math.PI/6,0),pt(G3.sph(.08),bm(0xffffff),0,.26,-.34));g.add(pt(G3.cyl(.02,.02,.9,6),sm(a,.4,.4),0,.18,0,0,0,Math.PI/2));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.tor(.5,.05),bm(0xfff2a0),0,.9,0,Math.PI/2,0,0));g.add(pt(G3.tor(.5,.1),bm(0xfff2a0,.25,true),0,.9,0,Math.PI/2,0,0));return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0x2a2a30,.2,.6);for(const sg of [-1,1]){g.add(pt(G3.cone(.2,.45,3),m,sg*.35,.25,0,0,0,-sg*.15));g.add(pt(G3.cone(.11,.28,3),bm(0xff9ac8),sg*.35,.22,-.03,0,0,-sg*.15));}return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0xf4f7ff,.1,.7);for(const sg of [-1,1]){g.add(pt(G3.cyl(.11,.14,.9,10),m,sg*.25,.5,0,0,0,-sg*.18));g.add(pt(G3.cyl(.06,.08,.7,10),bm(0xffa0cc),sg*.25,.5,-.05,0,0,-sg*.18));}return g;},
  (a,p)=>{const g=new T.Group(),m=sm(a,.5,.3);for(let i=0;i<6;i++)g.add(pt(G3.cone(.1,.42,4),m,0,.22,-.55+i*.22));return g;},
  (a,p)=>{const g=new T.Group(),k=sm(0x20242e,.4,.4);g.add(pt(G3.tor(.52,.06),k,0,.1,0));for(const sg of [-1,1])g.add(pt(G3.cyl(.2,.2,.14,14),sm(a,.5,.3),sg*.55,.1,0,0,0,Math.PI/2));return g;},
  (a,p)=>{const g=new T.Group(),m=GOLD();g.add(pt(G3.box(.45,.08,.45),m,0,.04,0),pt(G3.cyl(.06,.06,.3,8),m,0,.22,0),pt(G3.cyl(.3,.12,.4,14),m,0,.55,0),pt(G3.tor(.14,.03),m,.32,.55,0),pt(G3.tor(.14,.03),m,-.32,.55,0));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.03,.03,1.0,8),sm(0xcccccc,.7,.3),0,.5,0));const f=new T.Mesh(new T.PlaneGeometry(.6,.4),chkMat());f.position.set(.32,.82,0);g.add(f);return g;},
  (a,p)=>{const g=new T.Group(),r=sm(0xd01030,.3,.2);g.add(pt(G3.sph(.17),r,.14,.18,0),pt(G3.sph(.17),r,-.14,.18,0),pt(G3.cyl(.02,.02,.5,6),sm(0x2a8f2a,.1,.6),.1,.5,0,0,0,-.35),pt(G3.cyl(.02,.02,.5,6),sm(0x2a8f2a,.1,.6),-.1,.5,0,0,0,.35));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.5,.5,.18,18),sm(0xff9ac8,.1,.6),0,.1,0),pt(G3.cyl(.38,.38,.16,18),sm(0xffffff,.1,.6),0,.27,0),pt(G3.cyl(.04,.04,.25,8),sm(0xd01030,.1,.5),0,.48,0),pt(G3.sph(.06),bm(0xffe066),0,.65,0));return g;},
  (a,p)=>{const g=new T.Group(),sp=new T.Group();g.add(pt(G3.cyl(.2,.27,.12,14),sm(0x20242e,.5,.4),0,.06,0),pt(G3.cyl(.18,.18,.22,14),bm(0xff2a2a,.85),0,.23,0));sp.position.y=.23;sp.add(pt(G3.box(.7,.05,.12),bm(0xffffff,.9,true),0,0,0));g.add(sp);g.userData.spin=[{o:sp,ax:'y',v:9}];return g;},
  (a,p)=>{const g=new T.Group(),pr=new T.Group();g.add(pt(G3.cyl(.04,.04,.4,8),sm(0x9aa3b5,.6,.3),0,.2,0));pr.position.y=.42;pr.add(pt(G3.box(1.6,.03,.14),sm(0x20242e,.4,.4),0,0,0),pt(G3.box(.14,.03,1.6),sm(0x20242e,.4,.4),0,0,0));g.add(pr);g.userData.spin=[{o:pr,ax:'y',v:22}];return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.box(.42,.06,1.8),sm(a,.3,.4),0,.32,0,.15,0,0),pt(G3.box(.16,.07,1.7),sm(0xffffff,.2,.5),0,.34,0,.15,0,0),pt(G3.box(.05,.15,.2),sm(0x222222,.3,.5),0,.2,.7));return g;},
  (a,p)=>{const g=new T.Group(),m=sm(0x151515,.2,.6),d=pt(G3.sph(.45),m,0,.12,0);d.scale.set(1,.75,1);g.add(d,pt(G3.cyl(.6,.6,.04,18),m,0,.05,0));return g;}
];
/* ---------- antennes ---------- */
const ANTFN=[null,
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,1,8),sm(0x9aa3b5,.8,.3),0,.5,0),pt(G3.sph(.2),sm(a,.5,.3,a,.3),0,1.1,0));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,1,8),sm(0x9aa3b5,.8,.3),0,.5,0),pt(G3.cone(.2,.6,3),sm(a,.4,.4,a,.25),.3,.85,0,0,0,-Math.PI/2));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,1.1,8),sm(0x9aa3b5,.8,.3),0,.55,0),pt(G3.box(.55,.34,.02),sm(a,.4,.4,a,.2),.3,.85,0),pt(G3.box(.55,.1,.025),bm(0xffffff),.3,.85,0));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,.6,8),sm(0x9aa3b5,.8,.3),0,.3,0));const h=pt(shapeGeo('heart',s=>{s.moveTo(0,-.5);s.bezierCurveTo(-.15,-.35,-.5,-.1,-.5,.15);s.bezierCurveTo(-.5,.4,-.2,.5,0,.28);s.bezierCurveTo(.2,.5,.5,.4,.5,.15);s.bezierCurveTo(.5,-.1,.15,-.35,0,-.5);},.14),sm(0xff3b6b,.4,.3,0xff1a4a,.4),0,.95,0);h.scale.set(.5,.5,.5);g.add(h);return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,.6,8),sm(0x9aa3b5,.8,.3),0,.3,0));const st=pt(shapeGeo('star',s=>{for(let i=0;i<10;i++){const r=i%2?.22:.5,an=Math.PI/2+i*Math.PI/5;i?s.lineTo(Math.cos(an)*r,Math.sin(an)*r):s.moveTo(Math.cos(an)*r,Math.sin(an)*r);}s.closePath();},.12),sm(0xffd400,.6,.3,0xffb400,.4),0,.95,0);st.scale.set(.6,.6,.6);g.add(st);return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,.5,8),sm(0x9aa3b5,.8,.3),0,.25,0));const b=pt(shapeGeo('bolt',s=>{[[.1,.5],[-.3,-.05],[-.02,-.05],[-.12,-.5],[.3,.1],[.02,.1]].forEach((q,i)=>i?s.lineTo(q[0],q[1]):s.moveTo(q[0],q[1]));s.closePath();},.1),bm(0xffe14a),0,.95,0);b.scale.set(.8,.8,.8);g.add(b);return g;},
  (a,p)=>{const g=new T.Group(),b=pt(G3.sph(.28),sm(a,.5,.2,a,.15),0,1.5,0);b.scale.set(1,1.25,1);g.add(b,pt(G3.cyl(.008,.008,1.2,4),bm(0xffffff),0,.6,0));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.box(.07,.9,.02),sm(0xdfe6f2,1,.1),0,.75,0),pt(G3.box(.32,.06,.06),GOLD(),0,.28,0),pt(G3.cyl(.03,.03,.22,8),sm(0x5a3a1a,.1,.7),0,.14,0),pt(G3.sph(.05),GOLD(),0,.02,0));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.04,.07,.9,8),sm(0x7a4a1a,.1,.7),0,.45,0,0,0,.1));for(let i=0;i<4;i++){const an=i*Math.PI/2;g.add(pt(G3.cone(.1,.55,4),sm(0x2aa84a,.1,.6),Math.cos(an)*.3,.95,Math.sin(an)*.3,Math.sin(an)*1.1,0,-Math.cos(an)*1.1));}return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,.6,8),sm(0x9aa3b5,.8,.3),0,.3,0));const b=pt(G3.sph(.25),sm(0xff8a1e,.3,.4),0,.9,0);b.scale.set(1.5,.9,.6);g.add(b,pt(G3.cone(.2,.3,3),sm(0xff8a1e,.3,.4),-.45,.9,0,0,0,Math.PI/2),pt(G3.sph(.04),bm(0x111111),.25,.95,.12));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.1,.1,.55,12),sm(0xf4f7ff,.5,.3),0,.55,0),pt(G3.cone(.1,.25,12),sm(0xd01030,.4,.3),0,.95,0));for(let i=0;i<3;i++){const an=i*Math.PI*2/3;g.add(pt(G3.box(.02,.2,.14),sm(a,.5,.3),Math.cos(an)*.13,.35,Math.sin(an)*.13,0,-an,0));}return g;},
  (a,p)=>{const g=new T.Group(),sp=new T.Group();g.add(pt(G3.cyl(.03,.04,1.0,8),sm(0x9aa3b5,.8,.3),0,.5,0));sp.position.set(0,1.05,.05);for(let i=0;i<4;i++){const an=i*Math.PI/2;sp.add(pt(G3.box(.1,.32,.02),sm(i%2?a:0xffffff,.3,.5),0,0,0));sp.children[i].position.set(Math.sin(an)*.18,Math.cos(an)*.18,0);sp.children[i].rotation.z=-an;}g.add(sp);g.userData.spin=[{o:sp,ax:'z',v:4}];return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cone(.12,.7,10),sm(0xff7a1a,.2,.5),0,.5,0,Math.PI,0,0));for(let i=0;i<3;i++)g.add(pt(G3.cone(.04,.3,5),sm(0x2aa84a,.1,.6),(i-1)*.07,.97,0,0,0,(i-1)*-.3));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.025,.03,.8,8),sm(0x9aa3b5,.8,.3),0,.4,0),pt(G3.cyl(.28,.05,.1,16),sm(0xe8eef8,.7,.2),0,.9,0,.5,0,0),pt(G3.sph(.05),sm(a,.5,.3,a,.5),0,.96,-.12));return g;},
  (a,p)=>{const g=new T.Group();g.add(pt(G3.cyl(.02,.02,.9,8),sm(0xf4f7ff,.1,.6),0,.45,0),pt(G3.tor(.2,.06),sm(a,.4,.4,a,.2),0,1.0,0),pt(G3.cyl(.2,.2,.02,16),sm(0xffffff,.2,.5),0,1.0,0,Math.PI/2,0,0));return g;}
];

/* ---------- décalques (dessinés en blanc, teintés par la couleur d'accent) ---------- */
function starPath(x,cx,cy,r){x.beginPath();for(let i=0;i<10;i++){const a=-Math.PI/2+i*Math.PI/5,rr=i%2?r*.45:r;i?x.lineTo(cx+Math.cos(a)*rr,cy+Math.sin(a)*rr):x.moveTo(cx+Math.cos(a)*rr,cy+Math.sin(a)*rr);}x.closePath();x.fill();}
const DRAW=[null,
  x=>{x.fillRect(0,42,256,16);x.fillRect(0,70,256,8);},
  x=>{x.fillRect(0,34,256,10);x.fillRect(0,52,256,18);x.fillRect(0,78,256,10);},
  x=>{for(let k=0;k<7;k++){const bx=20+k*34;x.beginPath();x.moveTo(bx,110);x.quadraticCurveTo(bx+10,70-k*4,bx+30,40+k*6);x.quadraticCurveTo(bx+28,80,bx+44,110);x.closePath();x.fill();}},
  x=>{x.beginPath();x.moveTo(30,20);x.lineTo(130,20);x.lineTo(95,60);x.lineTo(170,60);x.lineTo(60,112);x.lineTo(98,70);x.lineTo(40,70);x.closePath();x.fill();},
  x=>{for(let i=0;i<16;i++)for(let j=0;j<4;j++)if((i+j)%2)x.fillRect(i*16,28+j*18,16,18);},
  x=>{x.lineWidth=10;x.lineCap='round';x.beginPath();x.moveTo(10,90);x.bezierCurveTo(60,10,110,120,160,40);x.bezierCurveTo(190,0,220,50,250,30);x.stroke();x.lineWidth=6;x.beginPath();x.moveTo(20,110);x.bezierCurveTo(80,50,140,110,230,70);x.stroke();},
  x=>{for(let r=0;r<5;r++)for(let c=0;c<12;c++){const cx=c*24+(r%2?12:0),cy=14+r*24;x.globalAlpha=Math.max(.15,1-c/12);x.beginPath();for(let k=0;k<6;k++){const a=k*Math.PI/3;k?x.lineTo(cx+Math.cos(a)*10,cy+Math.sin(a)*10):x.moveTo(cx+Math.cos(a)*10,cy+Math.sin(a)*10);}x.closePath();x.fill();}x.globalAlpha=1;},
  x=>{for(let i=0;i<9;i++)starPath(x,24+i*28,30+((i*37)%60),9+(i%3)*5);},
  x=>{for(let r=0;r<4;r++)for(let c=0;c<14;c++){x.beginPath();x.arc(10+c*18+(r%2?9:0),20+r*26,3+((c+r)%3)*2.5,0,7);x.fill();}},
  x=>{x.lineWidth=7;for(let r=0;r<4;r++){x.beginPath();for(let i=0;i<=256;i+=8){const y=22+r*24+Math.sin(i/22+r)*9;i?x.lineTo(i,y):x.moveTo(i,y);}x.stroke();}},
  x=>{const rg=mulberry(5);for(let i=0;i<26;i++){x.globalAlpha=.4+rg()*.6;x.beginPath();x.ellipse(rg()*256,rg()*128,10+rg()*26,6+rg()*14,rg()*3,0,7);x.fill();}x.globalAlpha=1;},
  x=>{x.lineWidth=4;x.beginPath();x.moveTo(0,40);x.lineTo(80,40);x.lineTo(100,60);x.lineTo(180,60);x.moveTo(0,80);x.lineTo(60,80);x.lineTo(80,100);x.lineTo(200,100);x.lineTo(220,70);x.lineTo(256,70);x.stroke();[[80,40],[180,60],[60,80],[200,100]].forEach(p=>{x.beginPath();x.arc(p[0],p[1],7,0,7);x.fill();});},
  x=>{x.lineWidth=14;x.lineCap='round';for(let k=0;k<3;k++){x.beginPath();x.moveTo(70+k*40,20);x.lineTo(130+k*40,108);x.stroke();}},
  x=>{for(const sg of [0,1]){x.beginPath();const o=sg*90;x.moveTo(10,70);for(let k=0;k<5;k++){x.lineTo(60+k*28,30+k*4+o*.2);x.lineTo(60+k*28+12,40+k*10+o*.2);}x.lineTo(230,96);x.closePath();x.globalAlpha=sg?.7:1;x.fill();}x.globalAlpha=1;},
  x=>{x.font='bold 110px system-ui,Arial';x.textAlign='center';x.textBaseline='middle';x.fillText('7',128,66);},
  x=>{x.lineWidth=12;for(let i=-4;i<12;i++){x.beginPath();x.moveTo(i*30,128);x.lineTo(i*30+60,0);x.stroke();}},
  x=>{for(let i=0;i<8;i++){x.beginPath();x.moveTo(i*32,100);x.lineTo(i*32+16,30);x.lineTo(i*32+32,100);x.closePath();x.fill();}},
  x=>{x.lineWidth=3;for(let r=0;r<5;r++)for(let c=0;c<10;c++){x.beginPath();x.arc(c*28+(r%2?14:0),r*22+20,14,Math.PI,0);x.stroke();}},
  x=>{x.lineWidth=4;for(let r=20;r<=60;r+=20){x.beginPath();x.arc(128,64,r,0,7);x.stroke();}x.beginPath();x.moveTo(128,0);x.lineTo(128,128);x.moveTo(40,64);x.lineTo(216,64);x.stroke();},
  x=>{const rg=mulberry(9);for(let i=0;i<60;i++){x.globalAlpha=.4+rg()*.6;x.fillRect(Math.floor(rg()*16)*16,Math.floor(rg()*8)*16,16,16);}x.globalAlpha=1;},
  x=>{x.lineWidth=8;x.lineCap='round';x.beginPath();x.moveTo(20,100);x.bezierCurveTo(60,10,90,120,130,30);x.bezierCurveTo(150,90,200,10,240,90);x.stroke();const rg=mulberry(3);for(let i=0;i<80;i++){x.beginPath();x.arc(rg()*256,rg()*128,1+rg()*2.5,0,7);x.fill();}},
  x=>{x.fillRect(0,30,256,12);x.fillRect(0,86,256,12);x.beginPath();x.arc(128,64,26,0,7);x.fill();x.globalCompositeOperation='destination-out';x.font='bold 36px system-ui,Arial';x.textAlign='center';x.textBaseline='middle';x.fillText('1',128,66);x.globalCompositeOperation='source-over';},
  x=>{const g=x.createLinearGradient(0,0,256,0);g.addColorStop(0,'rgba(255,255,255,0)');g.addColorStop(1,'rgba(255,255,255,1)');x.fillStyle=g;x.fillRect(0,20,256,88);}
];
const DTEX={};
function decalTex(i){
  if(DTEX[i])return DTEX[i];
  const [c,x]=mkc(256,128);x.clearRect(0,0,256,128);x.fillStyle='#fff';x.strokeStyle='#fff';
  if(DRAW[i])DRAW[i](x);
  const t=new T.CanvasTexture(c);DTEX[i]=t;return t;
}
function decalMat(i,acc){
  const key='d|'+i+'|'+acc;let m=MC.get(key);
  if(!m){m=new T.MeshStandardMaterial({map:decalTex(i),color:acc,transparent:true,depthWrite:false,side:T.DoubleSide,roughness:.4,metalness:.3,polygonOffset:true,polygonOffsetFactor:-2,polygonOffsetUnits:-2});MC.set(key,m);}
  return m;
}

/* ---------- la voiture complète ---------- */
function makeCarMesh(L,team,free){
  const P=carParams(L.car),g=new T.Group();
  const prim=free?hexN(COLORS[L.c1][1]):TEAM[team],acc=hexN(COLORS[L.c2][1]);
  const fin=FINS[L.fin]||FINS[1];
  const paint=sm(prim,fin[1],fin[2]);
  const glass=sm(P.tint,.85,.12,0x04070f,1),dark=sm(0x14161c,.2,.7),accM=sm(acc,.6,.3,acc,.25);
  const w=P.w,body=P.body,cab=P.cab;
  g.add(new T.Mesh(cg('body'+L.car,()=>extr(body,w,.12)),paint));
  // habitacle
  const rx1=cab[3][0],rx2=cab[2][0],ry=cab[2][1],rl=Math.abs(rx2-rx1);
  if(P.cv==='bubble'){
    const b=pt(G3.sph(.5),glass,0,ry-.22,-(rx1+rx2)/2);b.scale.set(w*.36,(ry-cab[0][1])*1.2,rl*.62);g.add(b);
  }else{
    g.add(new T.Mesh(cg('cab'+L.car,()=>extr(cab,w-.35,.08)),glass));
    if(P.cv!=='open'){
      g.add(pt(G3.box(w-.55,.07,rl+.12),paint,0,ry+.07,-(rx1+rx2)/2));
      if(!L.decal)g.add(pt(G3.box(.32,.02,rl+.14),accM,0,ry+.115,-(rx1+rx2)/2));
    }
  }
  if(!L.decal)g.add(pt(G3.box(.32,.02,1.5),accM,0,body[3][1]+.1,-(body[3][0]-.6),.12,0,0));
  // décalque
  if(L.decal){
    const dm=decalMat(L.decal,acc),len=(P.xmax-P.xmin)*.62;
    for(const sg of [-1,1]){
      const pl=new T.Mesh(cg('dpl'+len.toFixed(2),()=>new T.PlaneGeometry(len,.62)),dm);
      pl.position.set(sg*(w/2+.03),-.13,-(P.xmax+P.xmin)/2);pl.rotation.y=sg*Math.PI/2;if(sg<0)pl.scale.x=-1;g.add(pl);
    }
    if(P.cv!=='open'&&P.cv!=='bubble'){
      const rp=new T.Mesh(cg('drp'+(w-.7).toFixed(2)+'|'+rl.toFixed(2),()=>new T.PlaneGeometry(w-.7,rl+.1)),dm);
      rp.rotation.x=-Math.PI/2;rp.rotation.z=Math.PI/2;rp.position.set(0,ry+.12,-(rx1+rx2)/2);g.add(rp);
    }
  }
  // roues
  const wr=P.wr,wb=P.wb,wd=IT['wheel:'+L.wheel].wd,wy=-.9+wr;
  const wheels=[];
  for(const sx of [-1,1])for(const sz of [-1,1]){
    const piv=new T.Group();piv.position.set(sx*(w/2-.02),wy,sz*wb);
    const spin=new T.Group();piv.add(spin);
    const wg=wheelGroup(wd,wr,acc);if(sx<0)wg.rotation.y=Math.PI;spin.add(wg);
    g.add(piv);wheels.push({piv:piv,spin:spin,front:sz<0});
  }
  const arch=bm(0x05060a);
  for(const sx of [-1,1])for(const sz of [-1,1])g.add(pt(G3.box(.06,wr*1.12,wr*2.1),arch,sx*(w/2+.02),wy+.1,sz*wb));
  // lumières
  const nose=body[2],tailp=body[6],hl=bm(P.head),tl=bm(P.tail);
  for(const sx of [-1,1]){
    g.add(pt(G3.box(.45,.12,.08),hl,sx*w*.32,nose[1]+.1,-(nose[0]+.02)));
    g.add(pt(G3.box(.45,.12,.08),tl,sx*w*.32,tailp[1]+.02,-tailp[0]+.02));
  }
  const beam=new T.Mesh(new T.PlaneGeometry(2.2,9),new T.MeshBasicMaterial({color:P.head,transparent:true,opacity:.07,blending:T.AdditiveBlending,depthWrite:false,side:T.DoubleSide}));
  beam.rotation.x=-Math.PI/2;beam.position.set(0,-.7,-(nose[0]+4.4));g.add(beam);
  // spoiler
  const y5=body[5][1],zS=-tailp[0]-.25;
  switch(P.sp){
    case 1:g.add(pt(G3.box(w-.3,.07,.3),paint,0,y5+.1,zS,-.12,0,0));break;
    case 2:g.add(pt(G3.box(w-.1,.07,.45),paint,0,y5+.4,zS));for(const sx of [-1,1])g.add(pt(G3.box(.08,.3,.2),dark,sx*.6,y5+.25,zS));break;
    case 3:g.add(pt(G3.box(w-.1,.07,.7),paint,0,y5+.62,zS));for(const sx of [-1,1])g.add(pt(G3.box(.08,.5,.2),dark,sx*.6,y5+.35,zS));break;
    case 4:g.add(pt(G3.box(w-.1,.06,.4),paint,0,y5+.4,zS),pt(G3.box(w-.1,.06,.4),accM,0,y5+.66,zS-.08));for(const sx of [-1,1])g.add(pt(G3.box(.08,.5,.2),dark,sx*.55,y5+.3,zS));break;
    case 5:g.add(pt(G3.box(w,.08,.55),paint,0,y5+.55,zS));for(const sx of [-1,1]){g.add(pt(G3.box(.06,.3,.6),accM,sx*w/2,y5+.55,zS));g.add(pt(G3.box(.08,.4,.2),dark,sx*.5,y5+.3,zS));}break;
  }
  // détails
  const zF=-(nose[0]);
  if(P.split)g.add(pt(G3.box(w-.1,.05,.6),dark,0,-.5,zF+.1));
  if(P.scoop){g.add(pt(G3.box(.5,.12,.7),dark,0,body[3][1]+.1,-(body[3][0]-.4)));g.add(pt(G3.box(.4,.05,.55),accM,0,body[3][1]+.17,-(body[3][0]-.4)));}
  if(P.rscoop&&P.cv==='glass')g.add(pt(G3.box(.3,.1,.5),dark,0,ry+.17,-(rx1+rx2)/2));
  if(P.fins)for(const sx of [-1,1])g.add(pt(G3.box(.05,.45,.8),paint,sx*.55,y5+.3,zS-.15,.35,0,0));
  if(P.pods)for(const sx of [-1,1])g.add(pt(G3.box(.1,.18,2.2),dark,sx*(w/2+.02),-.42,0));
  if(P.flare)for(const sx of [-1,1])for(const sz of [-1,1])g.add(pt(G3.box(.14,.1,wr*2.2),paint,sx*(w/2+.03),wy+wr*.85,sz*wb));
  for(let k=0;k<P.exh;k++){const xx=(k%2?1:-1)*(.35+(k>>1)*.3);g.add(pt(G3.cyl(.07,.07,.3,8),sm(0x9aa3b5,.9,.3),xx,tailp[1]-.18,-tailp[0]+.05,Math.PI/2,0,0));}
  if(P.lbar&&P.cv==='glass')g.add(pt(G3.box(w-.7,.08,.08),bm(P.head),0,ry+.12,-rx2-.05));
  // néon sous la caisse
  const ug=new T.Mesh(new T.PlaneGeometry(w+.5,4.6),new T.MeshBasicMaterial({color:prim,transparent:true,opacity:.5,blending:T.AdditiveBlending,depthWrite:false,side:T.DoubleSide}));
  ug.rotation.x=-Math.PI/2;ug.position.y=-.78;g.add(ug);
  // topper + antenne
  const ud={wheels:wheels,spinA:0,spin:[],hb:HB[P.hb],L:L};
  if(L.topper&&TOPFN[L.topper]){
    const tg=TOPFN[L.topper](acc,prim);tg.position.set(0,ry+.1,-(rx1+rx2)/2);tg.scale.setScalar(Math.min(1.15,w/2));g.add(tg);
    if(tg.userData.spin)tg.userData.spin.forEach(s=>ud.spin.push(s));
  }
  if(L.ant&&ANTFN[L.ant]){
    const ag=ANTFN[L.ant](acc,prim);ag.position.set(w*.28,body[5][1]+.02,-body[5][0]*.9);g.add(ag);
    if(ag.userData.spin)ag.userData.spin.forEach(s=>ud.spin.push(s));
  }
  // flammes (couleur du boost choisi)
  const bd=IT['boost:'+L.boost].bd,c1=hexN(bd.c1),c2=hexN(bd.c2);
  const zr=-tailp[0]+.1;
  const fg=new T.ConeGeometry(.5,3.2,10);fg.translate(0,1.6,0);fg.rotateX(Math.PI/2);
  const flame=new T.Mesh(fg,new T.MeshBasicMaterial({color:c1,transparent:true,opacity:.85,blending:T.AdditiveBlending,depthWrite:false}));
  flame.position.set(0,-.1,zr);flame.visible=false;g.add(flame);
  const fg2=new T.ConeGeometry(.28,2,8);fg2.translate(0,1,0);fg2.rotateX(Math.PI/2);
  const flame2=new T.Mesh(fg2,new T.MeshBasicMaterial({color:c2,transparent:true,opacity:.95,blending:T.AdditiveBlending,depthWrite:false}));
  flame2.position.set(0,-.1,zr);flame2.visible=false;g.add(flame2);
  ud.flame=flame;ud.flame2=flame2;ud.wr=wr;ud.zr=zr;ud.wy=wy;ud.wb=wb;ud.w=w;
  g.userData=ud;
  return g;
}
function spinTick(m,dt){const s=m.userData&&m.userData.spin;if(s)for(let i=0;i<s.length;i++)s[i].o.rotation[s[i].ax]+=s[i].v*dt;}

/* =====================================================================
   ÉTAT DU JEU
   ===================================================================== */
let cars=[],player=null,scores=[0,0],timeLeft=180,overtime=false,otTime=0;
let phase='menu',phaseT=0,countT=0,msgT=0,lastTeamScored=0,goalFrame=0;
let showcase=null,showcaseT=0;
/* positions de coup d'envoi du vrai RL : coins, diagonales arrière, centre arrière */
const SP={1:[[19,31.5]],2:[[-19,31.5],[19,31.5]],3:[[-19,31.5],[19,31.5],[0,54]],4:[[-19,31.5],[19,31.5],[-2.5,45],[2.5,45]]};
const tA=V3(),tB=V3(),tC=V3(),tD=V3(),tE=V3(),tF=V3();
const qa=new T.Quaternion(),mtx=new T.Matrix4();

function newCar(team,slot,human,name,skill){
  const c={team:team,slot:slot,human:human,name:name,skill:skill,
    pos:V3(),vel:V3(),q:new T.Quaternion(),fwd:V3(0,0,-1),up:V3(0,1,0),right:V3(1,0,0),gn:V3(0,1,0),av:V3(),flipAxis:V3(),jumpUp:V3(0,1,0),
    boost:33,grounded:false,ghost:false,coach:false,dodgeAge:9,flipF:0,flipR:0,prevJ:false,noGround:0,jumpT:0,flipClock:0,flipAvail:false,flipT:0,flipRate:0,hitBoost:0,boosting:false,
    dead:false,deadT:0,stuckT:0,role:0,eta:0,think:0,boostOK:true,hitCool:0,aerialCool:0,aerial:false,aerialT:0,botFlip:0,stuck:0,revT:0,revS:1,
    in:{x:0,y:0,thr:0,boost:false,jump:false,jumpEdge:false,drift:false,rollMode:false,roll:0,pwEdge:false},
    pw:'',magT:0,spkT:0,hayT:0,phT:0,demT:0,frzT:0,plgT:0,grpT:0,scr:0,spkHold:0,pwCool:0,pwWait:0,beam:null,
    stats:{goals:0,assists:0,saves:0,shots:0,demos:0,score:0},mesh:null,label:null,shadow:null,L:null,hb:HB.std,bfx:null,tfx:null,ed:null};
  const L=human?SV.load:randLoad(rnd);
  c.L=L;
  c.mesh=makeCarMesh(L,team,human&&MUT.freeColors);
  c.hb=c.mesh.userData.hb;c.bfx=boostFx(L.boost);c.tfx=trailFx(L.trail);c.ed=IT['expl:'+L.expl].ed;
  scene.add(c.mesh);
  if(!human){c.label=makeLabel(name,TCSS[team]);c.label.position.set(0,2.4,0);c.mesh.add(c.label);}
  c.shadow=new T.Mesh(new T.CircleGeometry(1,20),new T.MeshBasicMaterial({color:0,transparent:true,opacity:.4,depthWrite:false}));
  c.shadow.rotation.x=-Math.PI/2;scene.add(c.shadow);
  return c;
}
function placeCar(c){
  const sp=SP[cfg.size][c.slot],sz=c.team===0?-1:1,sx=c.team===0?1:-1;
  c.pos.set(sp[0]*sx*HX/40,CR+.02,sz*sp[1]*HZ/60);if(DROP.on)hexFit(c.pos,3);c.vel.set(0,0,0);
  c.q.setFromAxisAngle(V3(0,1,0),c.team===0?Math.PI:0);
  c.fwd.set(0,0,-1).applyQuaternion(c.q);c.up.set(0,1,0);c.right.set(1,0,0).applyQuaternion(c.q);
  c.av.set(0,0,0);c.boost=MUT.boost==='none'?0:(MUT.boost==='inf'?100:33);c.grounded=true;c.noGround=0;c.jumpT=0;c.flipT=0;c.flipAvail=false;c.hitBoost=0;c.boosting=false;
  c.dead=false;c.mesh.visible=true;c.stuckT=0;c.aerial=false;c.revT=0;c.stuck=0;
  resetPowerState(c);
  const I=c.in;I.x=I.y=I.thr=I.roll=0;I.boost=I.jump=I.jumpEdge=I.drift=I.rollMode=I.pwEdge=false;
}
function resetBall(){
  ball.pos.set(0,BR+.05,0);ball.vel.set(0,0,0);ball.alive=true;ball.mesh.visible=true;ball.last=null;ball.prev=null;ball.shotTeam=-1;
  ball.frozen=0;ball.held=null;ball.py=ball.pos.y;
  HS.team=-1;HS.speed=34;DROP.charge=0;DROP.team=-1;
  ball.mesh.position.copy(ball.pos);ball.halo.material.color.set(0xffffff);
  if(MODE.park){ball.alive=false;ball.mesh.visible=false;} /* parkour : pas de balle */
  if(MODE.run&&GR.on)grBallPos(); /* parcours de buts : la balle se replace devant le but actif */
}


/* =====================================================================
   EFFETS : boosts, traînées, explosions de but, hymnes
   ===================================================================== */
const fxRings=[];
function fxScene(){return phase==='menu'?lobbyScene:scene;}
function hsl2rgb(h,s,l){
  const a=s*Math.min(l,1-l),f=n=>{const k=(n+h*12)%12;return l-a*Math.max(-1,Math.min(k-3,9-k,1));};
  return [f(0),f(8),f(4)];
}
function cssRGB(s){const n=hexN(s);return [((n>>16)&255)/255,((n>>8)&255)/255,(n&255)/255];}
function boostFx(i){const bd=IT['boost:'+i].bd;return {k:bd.k,c1:cssRGB(bd.c1),c2:cssRGB(bd.c2)};}
function trailFx(i){const td=IT['trail:'+i].td;return {k:td.k,cols:td.col.map(cssRGB)};}
const _bfxTmp=new T.Vector3();
function boostEmit(c,sup){
  const f=c.bfx,fw=c.fwd;
  _bfxTmp.copy(c.pos).addScaledVector(fw,-2.3);
  let col=f.c1,sp=9,sprd=3,life=.32,gr=8;
  switch(f.k){
    case 'rainbow':col=hsl2rgb((performance.now()/300+rnd())%1,1,.55);break;
    case 'spark':col=rnd()<.5?f.c1:f.c2;sprd=6;break;
    case 'smoke':col=rnd()<.5?f.c1:f.c2;life=.7;gr=-1;sp=5;break;
    case 'bubble':col=rnd()<.5?f.c1:f.c2;gr=-3;life=.7;sprd=4;break;
    case 'confetti':col=hsl2rgb(rnd(),1,.6);sprd=7;gr=10;life=.6;break;
    case 'bolt':col=rnd()<.5?f.c1:f.c2;sprd=9;life=.18;break;
    case 'ghost':col=f.c1;life=.55;gr=-2;sp=4;break;
    case 'pixel':col=rnd()<.5?f.c1:f.c2;sprd=2;life=.4;break;
    case 'snow':col=f.c2;gr=3;life=.8;sp=4;sprd=5;break;
    case 'heart':col=rnd()<.5?f.c1:f.c2;gr=-2;life=.7;sp=5;sprd=4;break;
    default:col=sup?[.7,.8,1]:f.c1;
  }
  emit(_bfxTmp.x,_bfxTmp.y,_bfxTmp.z,-fw.x*sp+(rnd()-.5)*sprd,-fw.y*sp+(rnd()-.5)*sprd,-fw.z*sp+(rnd()-.5)*sprd,col[0],col[1],col[2],life,gr);
}
function trailEmit(c){
  const tf=c.tfx;if(!tf||tf.k==='none'||!c.mesh.visible)return;
  if(c.vel.length()<9)return;
  const ud=c.mesh.userData;
  for(let s=-1;s<=1;s+=2){
    if(rnd()>.5)continue;
    _bfxTmp.copy(c.pos).addScaledVector(c.fwd,ud.wb*.8).addScaledVector(c.right,s*ud.w*.5).addScaledVector(c.up,-.7);
    let col=tf.cols.length?tf.cols[(rnd()*tf.cols.length)|0]:[1,1,1],life=.7,gr=0,vy=.6+rnd()*1.4;
    switch(tf.k){
      case 'rainbow':col=hsl2rgb((performance.now()/250+s*.1)%1,1,.55);break;
      case 'rainbow2':col=hsl2rgb((performance.now()/400)%1,.6,.75);break;
      case 'fire':life=.5;gr=-2;break;
      case 'smoke':life=1;gr=-1;break;
      case 'bubble':gr=-3;life=1;break;
      case 'heart':gr=-2;life=.9;break;
      case 'bolt':life=.25;vy=2;break;
      case 'pixel':life=.5;break;
      default:break;
    }
    const bk=c.fakeV||0;
    emit(_bfxTmp.x,_bfxTmp.y,_bfxTmp.z,(rnd()-.5)*1.6-c.fwd.x*bk,vy-c.fwd.y*bk,(rnd()-.5)*1.6-c.fwd.z*bk,col[0],col[1],col[2],life,gr);
  }
}
/* ----- anneaux d'onde de choc ----- */
function addRing(p,col,max,life,fs){
  const m=new T.Mesh(new T.RingGeometry(.92,1,40),new T.MeshBasicMaterial({color:col,transparent:true,opacity:1,blending:T.AdditiveBlending,depthWrite:false,side:T.DoubleSide,fog:false}));
  m.position.copy(p);(fs||fxScene()).add(m);fxRings.push({m:m,t:0,life:life,max:max,sc:fs||fxScene()});
}
function updRings(dt){
  for(let i=fxRings.length-1;i>=0;i--){
    const r=fxRings[i];r.t+=dt;const k=r.t/r.life;
    if(k>=1){r.sc.remove(r.m);r.m.geometry.dispose();r.m.material.dispose();fxRings.splice(i,1);continue;}
    r.m.scale.setScalar(1+k*r.max);r.m.material.opacity=1-k;r.m.lookAt(camera.position);
  }
}
/* ----- explosions de but ----- */
function colNum(s,team){return s==='t'?TEAM[team]:hexN(s);}
function runExplosion(ed,team,p){
  const cols=ed.col.map(s=>colNum(s,team)),c0=cols[0],c1=cols[1]||0xffffff;
  const q=V3(p.x,p.y,p.z),sc=fxScene();
  const later=(ms,fn)=>setTimeout(()=>{try{fn();}catch(e){}},ms);
  switch(ed.pat){
    case 'firework':
      burst(q,c0,90,30,1.4,6);
      for(let i=0;i<7;i++)later(150+i*170,()=>{const pp=V3(q.x+(rnd()-.5)*16,q.y+3+rnd()*9,q.z+(rnd()-.5)*8);burst(pp,cols[i%cols.length],70,26,1.3,6);addRing(pp,cols[i%cols.length],6,.5,sc);});
      break;
    case 'confetti':
      for(let i=0;i<5;i++)later(i*90,()=>{for(let k=0;k<50;k++){const c=cols[(rnd()*cols.length)|0];burst(V3(q.x+(rnd()-.5)*10,q.y+rnd()*5,q.z+(rnd()-.5)*10),c,1,24,2.4,12);}});
      break;
    case 'ring':
      for(let i=0;i<4;i++)later(i*140,()=>addRing(q,i%2?c1:c0,26+i*6,.9,sc));
      burst(q,c0,80,34,1.2,8);
      break;
    case 'bolt':
      for(let i=0;i<10;i++)later(i*70,()=>{const pp=V3(q.x+(rnd()-.5)*14,q.y+(rnd()-.5)*8,q.z+(rnd()-.5)*6);burst(pp,rnd()<.5?c0:c1,26,48,.25,0);});
      addRing(q,c0,30,.5,sc);
      break;
    case 'spiral':
      for(let i=0;i<180;i++)later(i*6,()=>{const a=i*.35,c=cols[i%cols.length],r=((c>>16)&255)/255,g=((c>>8)&255)/255,b=(c&255)/255;
        emit(q.x,q.y+i*.05,q.z,Math.cos(a)*18,10+rnd()*6,Math.sin(a)*18,r,g,b,1.4,6);});
      break;
    case 'implode':
      for(let i=0;i<140;i++){const a=rnd()*6.283,u=rnd()*2-1,s=Math.sqrt(1-u*u),d=V3(Math.cos(a)*s,u,Math.sin(a)*s),c=cols[i%cols.length];
        emit(q.x+d.x*20,q.y+d.y*20,q.z+d.z*20,-d.x*40,-d.y*40,-d.z*40,((c>>16)&255)/255,((c>>8)&255)/255,(c&255)/255,.5,0);}
      later(520,()=>{burst(q,c0,120,50,1.2,2);addRing(q,c0,34,.7,sc);});
      break;
    case 'rain':
      for(let i=0;i<8;i++)later(i*120,()=>{for(let k=0;k<22;k++){const c=cols[(rnd()*cols.length)|0];emit(q.x+(rnd()-.5)*22,q.y+14,q.z+(rnd()-.5)*10,0,-8-rnd()*6,0,((c>>16)&255)/255,((c>>8)&255)/255,(c&255)/255,2,5);}});
      burst(q,c0,40,20,1,6);
      break;
    case 'smoke':burst(q,c0,120,12,2.4,-1);burst(q,c1,80,8,2.8,-1);break;
    case 'comet':burst(q,c0,100,62,.9,0);burst(q,c1,60,40,1.2,0);addRing(q,c0,18,.5,sc);break;
    case 'snow':burst(q,c0,150,10,2.8,2);burst(q,c1,80,14,2.4,3);break;
    case 'heart':burst(q,c0,90,15,1.8,-2);burst(q,c1,60,10,2,-2);break;
    case 'nova':
      burst(q,c0,200,60,1.6,0);burst(q,c1,120,48,1.3,0);
      for(let i=0;i<3;i++)later(i*160,()=>addRing(q,cols[i%cols.length],40+i*10,.9,sc));
      break;
    default:
      burst(q,c0,160,38,1.6);burst(q,c1,80,46,1.2);
      [[-6,3],[6,5],[0,8],[-3,2],[3,2]].forEach((o,i)=>later(150+i*160,()=>burst(V3(q.x+o[0],q.y+o[1],q.z),i%2?c0:c1,60,30,1.3)));
  }
}
/* ----- hymnes de but ----- */
function playAnthem(i){
  if(!AC)return;const it=IT['anth:'+i];if(!it||!it.nd)return;
  it.nd.forEach(n=>tone(n[0],n[0]*1.01,n[2]+.12,i===3||i===6?'square':'sawtooth',.06,n[1]));
}

/* =====================================================================
   MODES DE JEU, MUTATEURS, RUMBLE, HEATSEEKER, ENTRAÎNEMENT, RÉCOMPENSES
   ===================================================================== */
const MUT={speed:1,grav:1,ballSize:1,bounce:1,ballSpeed:1,boost:'norm',boostUse:1,demo:'on',limit:0,freeColors:false};
const PK={t:0,run:false,done:0,KEY:'rocketArena3D.parkBest'};
const MODE={park:false,k:'casual',rumble:false,heat:false,puck:false,hoops:false,drop:false,free:false,run:false,train:false,moves:false,ranked:false,tour:false};
const HS={team:-1,speed:34};
const pwA=new T.Vector3(),pwB=new T.Vector3();
const CUSTOM=[
  ['arena','ARÈNE',[['std','Stade'],['hoops','Hoops'],['drop','Dropshot'],['snow','Neige']]],
  ['rumble','RUMBLE',[[0,'Non'],[1,'Oui']]],
  ['heat','HEATSEEKER',[[0,'Non'],[1,'Oui']]],
  ['grav','GRAVITÉ',[[.5,'Basse'],[1,'Normale'],[1.5,'Haute'],[2.2,'Très haute']]],
  ['ball','TAILLE BALLE',[[.6,'Petite'],[1,'Normale'],[1.5,'Grande'],[2.4,'Géante']]],
  ['bounce','REBOND',[[.7,'Faible'],[1,'Normal'],[1.25,'Fort']]],
  ['bspeed','VITESSE BALLE',[[.6,'Lente'],[1,'Normale'],[1.3,'Rapide']]],
  ['boost','BOOST',[['norm','Normal'],['inf','Illimité'],['none','Aucun'],['rech','Recharge']]],
  ['speed','VITESSE JEU',[[.7,'Lent'],[1,'Normal'],[1.4,'Rapide']]],
  ['demo','DÉMOLITIONS',[['on','Activées'],['off','Désactivées'],['all','Tous (amis aussi)']]],
  ['limit','LIMITE DE BUTS',[[0,'Aucune'],[3,'3'],[5,'5'],[10,'10']]]
];
/* ---------- balle / palet ---------- */
function setBallStyle(p){
  if(!ball.pm){
    ball.pm=new T.Mesh(new T.CylinderGeometry(2.7,2.7,2.0,28),new T.MeshStandardMaterial({color:0x14161c,metalness:.4,roughness:.35,emissive:0x112244,emissiveIntensity:.35}));
    ball.sphere=ball.mesh;ball.puck=false;
  }
  if(!!ball.puck===!!p)return;
  const old=ball.mesh;scene.remove(old);
  ball.mesh=p?ball.pm:ball.sphere;ball.puck=!!p;scene.add(ball.mesh);ball.mesh.visible=old.visible;
  ball.mesh.position.copy(ball.pos);
}
function configureMode(k){
  const rk0=k==='ranked'||String(k).indexOf('rk_')===0;if(String(k).indexOf('rk_')===0)k=k.slice(3);
  const m=Object.assign({speed:1,grav:1,ball:1,bounce:1,bspeed:1,boost:'norm',demo:'on',limit:0,arena:'std',rumble:0,heat:0},k==='custom'?(cfg.mut||SV.menu.mut||{}):{});
  MODE.k=k;MODE.ranked=rk0;MODE.tour=k==='tour';
  let arena='std';
  MODE.rumble=k==='rumble'||(k==='custom'&&!!+m.rumble);
  MODE.heat=k==='heat'||(k==='custom'&&!!+m.heat);
  if(k==='hoops')arena='hoops';else if(k==='drop')arena='drop';else if(k==='snow')arena='snow';else if(k==='custom')arena=m.arena||'std';
  MODE.hoops=arena==='hoops';MODE.drop=arena==='drop';MODE.puck=arena==='snow';
  MODE.free=k==='free';MODE.train=k==='train';MODE.moves=k==='moves';
  if(MODE.drop||MODE.hoops)MODE.heat=false;
  Object.assign(MUT,{speed:+m.speed,grav:+m.grav,ballSize:+m.ball,bounce:+m.bounce,ballSpeed:+m.bspeed,boost:m.boost,boostUse:1,demo:m.demo,limit:+m.limit});
  if(MODE.free||MODE.train||MODE.moves){MUT.boost='inf';MUT.demo='off';}
  MUT.freeColors=MODE.free||MODE.train||MODE.moves;
  PH=(MODE.moves||MODE.train)?PHL:PHR;
  G=PH.g*MUT.grav;BR=2.6*MUT.ballSize;BMAX=PH.bmax*MUT.ballSpeed*(MODE.heat?1.2:1);
  ball.rest=.62*MUT.bounce*(MODE.puck?.4:1);
  setArena(arena,!!(CM.cur&&CM.cur.small)); /* toutes les arènes en taille réelle (plus de mini-arène pour l'entraînement / les mécaniques) */cfg.arena=arena;
  setBallStyle(MODE.puck);
  ball.sphere.scale.setScalar(BR/2.6);ball.pm.scale.setScalar(BR/2.6);
  HS.team=-1;HS.speed=34;
  DROP.charge=0;DROP.team=-1;
}
/* ---------- Heatseeker ---------- */
function hsTouch(c){
  if(!MODE.heat)return;
  const atk=c.team===0?1:-1;
  HS.team=c.team;
  HS.speed=Math.min(BMAX,Math.max(HS.speed*1.05,34,ball.vel.length()*.85));
  pwA.set(0,GH*.5,atk*HZ).sub(ball.pos).normalize();
  ball.vel.copy(pwA).multiplyScalar(HS.speed);
}
function hsSteer(dt){
  if(!MODE.heat||HS.team<0)return;
  const atk=HS.team===0?1:-1;
  pwA.set(0,GH*.5,atk*HZ).sub(ball.pos).normalize();
  pwB.copy(ball.vel);if(pwB.lengthSq()<1)pwB.copy(pwA);pwB.normalize().lerp(pwA,1-Math.exp(-2.4*dt)).normalize().multiplyScalar(HS.speed);
  ball.vel.copy(pwB);
}
/* ---------- Dropshot : charge de la balle ---------- */
function dropTouch(c,impact){
  if(!MODE.drop)return;
  const old=DROP.charge,oldTeam=DROP.team;
  /* toute touche charge la balle au niveau 1 ; une frappe franche d'un même camp ajoute un niveau */
  if(DROP.team===c.team&&DROP.charge>0){if(impact>8)DROP.charge=Math.min(3,DROP.charge+1);}
  else{DROP.team=c.team;DROP.charge=1;}
  if(c===player&&DROP.charge>old&&DROP.charge>1)popup('CHARGE NIVEAU '+DROP.charge,0,'#ffd166');
}
function dropBallColor(){
  const lv=DROP.charge;
  return lv<=0?0xffffff:(DROP.team===0?0x2f8bff:0xff8a2a);
}
/* =====================  RUMBLE : POWER-UPS  ===================== */
const PW={
  mag:{n:'Aimant',ic:'🧲'},spike:{n:'Pointes',ic:'📌'},hay:{n:'Haymaker',ic:'👊'},boot:{n:'Botte',ic:'🥾'},
  plung:{n:'Ventouse',ic:'🪠'},freeze:{n:'Glaçon',ic:'🧊'},swap:{n:'Échange',ic:'🔄'},grap:{n:'Grappin',ic:'🪝'},
  power:{n:'Power Hitter',ic:'💥'},torn:{n:'Tornade',ic:'🌪️'},disr:{n:'Perturbateur',ic:'📡'},demo:{n:'Démolisseur',ic:'💣'}
};
const PWK=Object.keys(PW);
const TORN=[],DISR=[];
function givePower(c){
  if(c.pw||c.pwCool>0)return;
  c.pw=PWK[(rnd()*PWK.length)|0];c.pwCool=6;c.pwWait=.4+rnd()*1.6;
  if(c===player){popup(PW[c.pw].n,0,'#ffd166');}
}
function ensureBeam(c){
  if(c.beam)return c.beam;
  const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute([0,0,0,0,0,0],3));
  c.beam=new T.Line(g,new T.LineBasicMaterial({color:0xffe066}));c.beam.frustumCulled=false;c.beam.visible=false;scene.add(c.beam);return c.beam;
}
function usePower(c){
  if(!c.pw||c.dead||!ball.alive)return;
  const k=c.pw;c.pw='';
  switch(k){
    case 'mag':c.magT=8;break;
    case 'spike':c.spkT=10;break;
    case 'hay':c.hayT=10;break;
    case 'power':c.phT=10;break;
    case 'demo':c.demT=10;break;
    case 'freeze':c.frzT=10;break;
    case 'plung':c.plgT=5;ensureBeam(c);break;
    case 'grap':c.grpT=1.2;ensureBeam(c);break;
    case 'boot':
      pwA.copy(ball.pos).sub(c.pos);{const d=pwA.length();
        if(d<22){pwA.divideScalar(d||1);pwA.y+=.3;ball.vel.addScaledVector(pwA,52);ball.last=c;ball.halo.material.color.set(TEAM[c.team]);}}
      for(const o of cars){if(o===c||o.dead||o.team===c.team)continue;pwB.copy(o.pos).sub(c.pos);const dd=pwB.length();if(dd<14){pwB.divideScalar(dd||1);pwB.y+=.3;o.vel.addScaledVector(pwB,34);}}
      burst(c.pos,0xffd166,26,16,.5);SFX.hit(22);break;
    case 'swap':
      pwA.copy(c.pos);c.pos.copy(ball.pos);c.pos.y=Math.max(c.pos.y,CR+.1);ball.pos.copy(pwA);ball.pos.y=Math.max(ball.pos.y,BR+.1);
      burst(c.pos,0xb967ff,30,16,.6);burst(ball.pos,0xb967ff,30,16,.6);break;
    case 'torn':{
      const m=new T.Mesh(new T.CylinderGeometry(9,2,24,18,1,true),new T.MeshBasicMaterial({color:0xbfe6ff,transparent:true,opacity:.35,blending:T.AdditiveBlending,depthWrite:false,side:T.DoubleSide,wireframe:true}));
      pwA.copy(c.pos).addScaledVector(c.fwd,16);pwA.y=12;m.position.copy(pwA);scene.add(m);
      TORN.push({x:pwA.x,z:pwA.z,t:5,m:m,team:c.team});break;}
    case 'disr':{
      const m=new T.Mesh(new T.CircleGeometry(14,32),new T.MeshBasicMaterial({color:0xb967ff,transparent:true,opacity:.28,blending:T.AdditiveBlending,depthWrite:false,side:T.DoubleSide}));
      m.rotation.x=-Math.PI/2;m.position.set(ball.pos.x,.12,ball.pos.z);scene.add(m);
      DISR.push({x:ball.pos.x,z:ball.pos.z,t:6,m:m,team:c.team});break;}
  }
  if(c===player){SFX.pad(true);popup(PW[k].n.toUpperCase(),0,'#ffd166');}
  feed(c.name+' : '+PW[k].n,TCSS[c.team]);
}
function clearPowers(){
  for(const t of TORN)scene.remove(t.m);for(const d of DISR)scene.remove(d.m);TORN.length=0;DISR.length=0;
  for(const c of cars){c.pw='';resetPowerState(c);}
  ball.frozen=0;ball.held=null;
}
function resetPowerState(c){
  c.magT=c.spkT=c.hayT=c.phT=c.demT=c.frzT=c.plgT=c.grpT=c.scr=0;c.spkHold=0;c.pwCool=0;c.pwWait=0;
  if(c.beam)c.beam.visible=false;
}
function releaseBall(c){
  if(ball.held!==c)return;
  ball.held=null;c.spkHold=0;
  ball.vel.copy(c.vel).addScaledVector(c.fwd,34);ball.vel.y+=3;
  ball.last=c;
}
function powerStep(c,dt){
  if(c.pwCool>0)c.pwCool-=dt;
  if(c.dead){if(ball.held===c)releaseBall(c);return;}
  if(c.magT>0){c.magT-=dt;if(ball.alive&&!ball.held){pwA.copy(c.pos).sub(ball.pos);const d=pwA.length();if(d<40&&d>3){pwA.divideScalar(d);ball.vel.addScaledVector(pwA,80*dt);ball.vel.multiplyScalar(1-1.3*dt);}}}
  if(c.plgT>0){c.plgT-=dt;if(ball.alive&&!ball.held){pwA.copy(c.pos).sub(ball.pos);const d=pwA.length();if(d>8){pwA.divideScalar(d);ball.vel.addScaledVector(pwA,70*dt);}}}
  if(c.grpT>0){c.grpT-=dt;if(ball.alive){pwA.copy(ball.pos).sub(c.pos);const d=pwA.length();if(d>4){pwA.divideScalar(d);c.vel.addScaledVector(pwA,110*dt);}}}
  if(c.spkT>0)c.spkT-=dt;if(c.hayT>0)c.hayT-=dt;if(c.phT>0)c.phT-=dt;if(c.demT>0)c.demT-=dt;if(c.frzT>0)c.frzT-=dt;if(c.scr>0)c.scr-=dt;
  if(ball.held===c){
    c.spkHold-=dt;
    if(c.in.jumpEdge||c.spkHold<=0){c.in.jumpEdge=false;releaseBall(c);}
  }
  if(c.in.pwEdge){c.in.pwEdge=false;usePower(c);}
}
function powerWorld(dt){
  for(let i=TORN.length-1;i>=0;i--){
    const t=TORN[i];t.t-=dt;t.m.rotation.y+=dt*6;
    if(t.t<=0){scene.remove(t.m);TORN.splice(i,1);continue;}
    const swirl=(p,v,k)=>{const dx=p.x-t.x,dz=p.z-t.z,d=Math.hypot(dx,dz);if(d<17&&p.y<26){
      v.x+=(-dz/(d+1)*45-dx*.9)*dt*k;v.z+=(dx/(d+1)*45-dz*.9)*dt*k;v.y+=22*dt*k;}};
    if(ball.alive&&!ball.held&&MODE.k!=='none')swirl(ball.pos,ball.vel,1);
    for(const c of cars)if(!c.dead&&c.team!==t.team)swirl(c.pos,c.vel,.6);
  }
  for(let i=DISR.length-1;i>=0;i--){
    const d=DISR[i];d.t-=dt;d.m.material.opacity=.18+.12*Math.sin(performance.now()/120);
    if(d.t<=0){scene.remove(d.m);DISR.splice(i,1);continue;}
    for(const c of cars)if(!c.dead&&c.team!==d.team&&Math.hypot(c.pos.x-d.x,c.pos.z-d.z)<14)c.scr=.25;
  }
}
function botPower(c,dt){
  if(!c.pw)return;
  c.pwWait-=dt;if(c.pwWait>0)return;
  pwA.copy(ball.pos).sub(c.pos);const d=pwA.length();
  const face=d>0?pwA.dot(c.fwd)/d:0;
  let use=false;
  switch(c.pw){
    case 'boot':use=d<9;break;
    case 'swap':use=d>35&&rnd()<.01;break;
    case 'grap':use=d>20&&face>.5;break;
    case 'torn':use=d<26&&face>.3;break;
    case 'disr':use=d<24;break;
    default:use=d<16&&face>.2;
  }
  if(use){c.in.pwEdge=true;c.pwWait=.5;}
}
function powerHUD(){
  const e=$('pw');if(!e)return;
  if(!MODE.rumble||!player||!player.pw){e.style.display='none';return;}
  e.style.display='block';e.textContent=PW[player.pw].ic+' '+PW[player.pw].n.toUpperCase()+(isTouch?'':'  [F / clic droit]');
}
function powerBeams(){
  for(const c of cars){
    if(!c.beam)continue;
    const on=(c.grpT>0||c.plgT>0)&&!c.dead&&ball.alive;
    c.beam.visible=on;
    if(on){const a=c.beam.geometry.attributes.position;a.setXYZ(0,c.pos.x,c.pos.y+.4,c.pos.z);a.setXYZ(1,ball.pos.x,ball.pos.y,ball.pos.z);a.needsUpdate=true;}
  }
}
/* =====================  ENTRAÎNEMENT  ===================== */
const TR={i:0,t:0,att:0,ok:0,state:'play',wait:0};
const PACKS=[
  {n:'Tirs au but',d:'Marque dans la cage depuis différentes positions.'},
  {n:'Gardien',d:'Arrête les tirs envoyés vers ta cage.'},
  {n:'Aériens',d:'La balle est en l\'air : saute et frappe-la.'},
  {n:'Rebonds au mur',d:'La balle remonte le mur : lis le rebond.'}
];
function trainSetup(){
  const p=player;if(!p)return;
  const pk=cfg.pack||0,hz=HZ,hx=HX;
  resetBall();p.boost=100;
  const place=(x,z,face)=>{p.pos.set(x,CR+.02,z);p.vel.set(0,0,0);p.q.setFromAxisAngle(V3(0,1,0),face?Math.PI:0);p.grounded=true;p.av.set(0,0,0);};
  if(pk===0){place((rnd()-.5)*30,-hz*.55-rnd()*8,true);ball.pos.set((rnd()-.5)*34,BR,-hz*.2+rnd()*14);ball.vel.set((rnd()-.5)*6,0,rnd()*10);}
  else if(pk===1){place((rnd()-.5)*10,-hz+14,true);ball.pos.set((rnd()-.5)*50,BR+rnd()*8,hz*.1+rnd()*hz*.3);
    pwA.set((rnd()-.5)*2*GW*.8,GH*.35,-hz-ball.pos.z).sub(V3(ball.pos.x,0,0));pwA.set((rnd()-.5)*GW*1.4-ball.pos.x,GH*.3-ball.pos.y,-hz+2-ball.pos.z);
    const d=pwA.length();ball.vel.copy(pwA).multiplyScalar((34+rnd()*14)/d);}
  else if(pk===2){place((rnd()-.5)*20,-8,true);ball.pos.set((rnd()-.5)*20,14+rnd()*4,hz*.1);ball.vel.set((rnd()-.5)*8,6,10+rnd()*8);}
  else{place(hx-18,-10,true);ball.pos.set(hx-3,6,-4);ball.vel.set(10,16,18+rnd()*8);}
  ball.mesh.position.copy(ball.pos);
  TR.t=0;TR.state='play';TR.wait=0;
  const f=p.fwd;camHead.set(0,0,1);
}
function trainResult(ok){
  if(TR.state!=='play')return;
  TR.state='wait';TR.wait=1.4;TR.att++;if(ok)TR.ok++;
  showMsg(ok?'RÉUSSI !':'RATÉ',TR.ok+' / '+TR.att,ok?'#7dffb0':'#ff8a8a',1.2);
}
function trainUpdate(dt){
  if(!MODE.train||!player)return;
  if(TR.state==='wait'){TR.wait-=dt;if(TR.wait<=0)trainSetup();return;}
  TR.t+=dt;
  const pk=cfg.pack||0;
  if(TR.t>12||(pk===1&&TR.t>3&&ball.vel.z>0&&ball.vel.length()>6&&ball.pos.z>-HZ*.8)||(TR.t>5&&ball.vel.length()<2.5))trainResult(pk===1&&ball.vel.length()>2&&ball.pos.z>-HZ+4);
}
/* =====================  PACK MÉCANIQUES : coach + entraînement  =====================
   Un bot « COACH » fait la démo du move, puis c'est à toi de le réussir (autant de fois que tu veux).
   Chaque mécanique a : une définition, les touches, un scénario de départ et une détection de réussite. */
const MV={i:0,state:'idle',t:0,att:0,ok:0,car:null,S:{},wait:0,coach:null,demo:true,demoOK:null,_hud:'',_msg:''};
/* PINCH : dernière balle coincée entre une voiture et le sol / un mur (renseigné par carBall, lu par la mécanique « Pinch ») */
const PINCH={t:-9,c:null,cs:0,k:''};
/* après un pinch, la même voiture ne re-frappe pas la balle pendant ce court délai (s) : sinon, coincée entre la voiture qui continue d'avancer et le mur, la balle est repoussée une 2e fois et perd presque tout son rebond vers l'intérieur du terrain */
const PINCH_FREE=.12;
const mA=V3(),mB=V3();
function mvPlace(c,x,y,z,a,sp){
  c.q.setFromAxisAngle(V3(0,1,0),Math.PI+a);
  c.pos.set(x,y,z);
  c.fwd.set(0,0,-1).applyQuaternion(c.q);c.up.set(0,1,0);c.right.set(1,0,0).applyQuaternion(c.q);
  c.vel.copy(c.fwd).multiplyScalar(sp||0);
  c.av.set(0,0,0);c.boost=100;c.grounded=y<CR+.4;c.gn.set(0,1,0);c.noGround=0;c.jumpT=0;c.flipT=0;c.flipAvail=false;c.flipClock=0;
  c.hitBoost=0;c.boosting=false;c.dodgeAge=9;c.dead=false;c.ghost=false;c.mesh.visible=true;c.stuckT=0;c.aerial=false;c.prevJ=false;
  const I=c.in;I.x=I.y=I.thr=I.roll=0;I.boost=I.jump=I.jumpEdge=I.drift=I.rollMode=I.pwEdge=false;
  c.mesh.position.copy(c.pos);c.mesh.quaternion.copy(c.q);
}
const mWB=new T.Matrix4();
/* voiture posée SUR le mur latéral (s=1 : mur de droite, s=-1 : mur de gauche), roues contre la paroi.
   pitch = angle de montée du nez (0 = le long du mur vers +z, π/2 = droit vers le haut) */
function mvPlaceWall(c,s,y,z,sp,pitch){
  const sn=Math.sin(pitch||0),cs=Math.cos(pitch||0);
  mvPlace(c,0,y,z,0,0);
  mWB.makeBasis(V3(0,-s*cs,s*sn),V3(-s,0,0),V3(0,-sn,-cs));
  c.q.setFromRotationMatrix(mWB);
  c.pos.set(s*(HX-CR-.05),y,z);
  c.fwd.set(0,0,-1).applyQuaternion(c.q);c.up.set(0,1,0).applyQuaternion(c.q);c.right.set(1,0,0).applyQuaternion(c.q);
  c.gn.set(-s,0,0);c.grounded=true;
  c.vel.copy(c.fwd).multiplyScalar(sp||0);
  c.mesh.position.copy(c.pos);c.mesh.quaternion.copy(c.q);
}
function mvBall(x,y,z,vx,vy,vz,hover){
  resetBall();ball.pos.set(x,y,z);ball.vel.set(vx||0,vy||0,vz||0);ball.frozen=hover?9999:0;ball.mesh.position.copy(ball.pos);
}
/* oriente le nez de la voiture vers un point (contrôle aérien du coach) */
function mvAim(c,I,tx,ty,tz){
  mA.set(tx,ty,tz).sub(c.pos);const d=mA.length()+1e-3;mA.divideScalar(d);
  const lx=mA.dot(c.right),ly=mA.dot(c.up),lf=mA.dot(c.fwd);
  I.y=lf<0?(ly>=0?1:-1):clamp(ly*3,-1,1);I.x=clamp(lx*3,-1,1);I.roll=clamp(c.right.y*2.5,-1,1);
  return {d:d,lf:lf};
}
const H_=(t,a,b)=>t>=a&&t<b;           // t dans [a,b[
const spd2=c=>c.vel.length();
const JIT=()=>(rnd()-.5);

/* PINCH (sol / mur) : détection commune. kind = 'sol' ou 'mur' (surface contre laquelle la balle est écrasée) */
function pinchTouch(c,S,t,kind){
  if(S.pin)return;
  if(PINCH.c===c&&clockT-PINCH.t<.02){
    if((PINCH.k==='sol')!==(kind==='sol')){S.msg=kind==='sol'?'Là, tu as coincé la balle contre un mur : ici, écrase-la contre le sol':'Là, tu as coincé la balle contre le sol : ici, écrase-la contre le mur';return false;}
    S.pin={t:t,cs:PINCH.cs};return;
  }
  if(S.t1===undefined)S.t1=t;
}
function pinchTick(S,t,o){
  if(!S.pin){if(S.t1!==undefined&&t>S.t1+.3){S.msg=o.miss;return false;}return;}
  if(t<S.pin.t+.16)return;
  if(!S.pw){
    const sp=ball.vel.length(),r=sp/Math.max(8,S.pin.cs);
    if(sp>=o.minSp&&r>=o.minR){
      if(!o.goal){S.msg='Pinch ! '+Math.round(sp*2.7)+' km/h ('+r.toFixed(1).replace('.',',')+' × ta vitesse)';return true;}
      S.pw=true;
    }else{S.msg='Pas assez écrasée : '+Math.round(sp*2.7)+' km/h seulement. '+o.weak;return false;}
  }
  if(t>S.pin.t+(o.goalT||3.2)){S.msg=o.noGoal;return false;}
}

/* démo du coach, flip reset « mur → plafond » : monte le long du mur, tape la balle, saute, demi air roll, touche avec le dessous.
   Renvoie true une fois le contact des 4 roues obtenu (S.rs posé par la mécanique). */
function mvCeilReset(c,t,I,S){
   if(S.rs!==undefined)return true;
   const YB=H/3,DJ=7,D1=10;   /* hauteur de balle où l'on lâche le boost, distance pour sauter, distance où l'on se retourne */
   const dx=ball.pos.x-c.pos.x,dy=ball.pos.y-c.pos.y,dz=ball.pos.z-c.pos.z,d=Math.hypot(dx,dy,dz)||1;
   if(S.jt===undefined){
     /* au sol puis sur le mur : on pousse la balle vers le haut avec le boost, puis on la lâche : elle file vers le plafond */
     I.thr=1;I.boost=c.boost>1&&c.vel.length()<32&&ball.pos.y<YB;
     I.x=clamp((dx*c.right.x+dy*c.right.y+dz*c.right.z)/d*3,-1,1);
     if(c.grounded&&c.up.y<.5&&ball.pos.y>YB+3&&d>DJ)S.jt=t;
     return false;
   }
   if(t-S.jt<.15){I.jump=true;I.thr=1;return false;}
   if(c.grounded)return false;
   if(d>D1){const r=mvAim(c,I,ball.pos.x,ball.pos.y,ball.pos.z);I.boost=r.lf>.8&&c.boost>1;}
   else{
     /* demi air roll : le dessous de la voiture (−up) doit viser la balle */
     const ux=-dx/d,uy=-dy/d,uz=-dz/d;
     const lx=ux*c.right.x+uy*c.right.y+uz*c.right.z,lf=ux*c.fwd.x+uy*c.fwd.y+uz*c.fwd.z,lu=ux*c.up.x+uy*c.up.y+uz*c.up.z;
     I.roll=clamp(lx*3,-1,1);if(Math.abs(lx)<.3&&lu<0)I.roll=1;
     I.y=clamp(-lf*3,-1,1);I.x=0;I.boost=false;
   }
   return false;
}
/*FRS-BEGIN*/
/* DÉMO SCRIPTÉE du flip reset : le coach est piloté image par image (sans physique de voiture) pour réussir à tous les coups :
   sol → mur (la balle roule le long de la paroi) → plafond → retombée lente → demi air roll → 4 roues sur la balle (reset) → frappe dans le but.
   La balle suit le chemin scripté tant qu'elle est « figée », puis la vraie physique reprend (retombée lente, tir, but). */
const FR={S:{d:0,nx:0,ny:0,nz:0},TJ:5.3,TREL:6.5,TAPP:1.5,TSHOT:.45};
const frSS=(a,b,x)=>{const t=Math.min(1,Math.max(0,(x-a)/(b-a)));return t*t*(3-2*t);};
function frProj(p,rad){for(let i=0;i<3;i++){sdf(p.x,p.y,p.z,FR.S,true);const pen=FR.S.d+rad;if(pen<=0)break;p.x-=FR.S.nx*pen;p.y-=FR.S.ny*pen;p.z-=FR.S.nz*pen;}}
function frQuat(q,r,u,b){
  const m00=r.x,m01=u.x,m02=b.x,m10=r.y,m11=u.y,m12=b.y,m20=r.z,m21=u.z,m22=b.z,tr=m00+m11+m22;let x,y,z,w;
  if(tr>0){const k=Math.sqrt(tr+1)*2;w=.25*k;x=(m21-m12)/k;y=(m02-m20)/k;z=(m10-m01)/k;}
  else if(m00>m11&&m00>m22){const k=Math.sqrt(1+m00-m11-m22)*2;w=(m21-m12)/k;x=.25*k;y=(m01+m10)/k;z=(m02+m20)/k;}
  else if(m11>m22){const k=Math.sqrt(1+m11-m00-m22)*2;w=(m02-m20)/k;x=(m01+m10)/k;y=.25*k;z=(m12+m21)/k;}
  else{const k=Math.sqrt(1+m22-m00-m11)*2;w=(m10-m01)/k;x=(m02+m20)/k;y=(m12+m21)/k;z=.25*k;}
  q.x=x;q.y=y;q.z=z;q.w=w;
}
/* repère de la voiture : up (haut) et fwd (avant) ; right = up × (−fwd), comme le reste du jeu */
function frBasis(u,f,out){
  let d=f.x*u.x+f.y*u.y+f.z*u.z,fx=f.x-u.x*d,fy=f.y-u.y*d,fz=f.z-u.z*d,l=Math.hypot(fx,fy,fz);
  if(l<1e-3){const c=Math.abs(u.z)<.9?[0,0,1]:[1,0,0];d=c[0]*u.x+c[1]*u.y+c[2]*u.z;fx=c[0]-u.x*d;fy=c[1]-u.y*d;fz=c[2]-u.z*d;l=Math.hypot(fx,fy,fz)||1;}
  fx/=l;fy/=l;fz/=l;const bx=-fx,by=-fy,bz=-fz;
  out.f={x:fx,y:fy,z:fz};out.b={x:bx,y:by,z:bz};out.r={x:u.y*bz-u.z*by,y:u.z*bx-u.x*bz,z:u.x*by-u.y*bx};
}
function frApply(c,px,py,pz,q){ /* pose la voiture : position + quaternion, et recalcule fwd / up / right */
  c.pos.set(px,py,pz);c.q.set(q.x,q.y,q.z,q.w);
  const rot=(x,y,z,o)=>{const ux=q.x,uy=q.y,uz=q.z,w=q.w,cx=uy*z-uz*y,cy=uz*x-ux*z,cz=ux*y-uy*x,dx=uy*cz-uz*cy,dy=uz*cx-ux*cz,dz=ux*cy-uy*cx;o.set(x+2*(w*cx+dx),y+2*(w*cy+dy),z+2*(w*cz+dz));};
  rot(0,0,-1,c.fwd);rot(0,1,0,c.up);rot(1,0,0,c.right);
}
function frQmul(a,b){return {x:a.w*b.x+a.x*b.w+a.y*b.z-a.z*b.y,y:a.w*b.y-a.x*b.z+a.y*b.w+a.z*b.x,z:a.w*b.z+a.x*b.y-a.y*b.x+a.z*b.w,w:a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z};}
function frNlerp(a,b,t){let d=a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;const s=d<0?-1:1;let x=a.x+(s*b.x-a.x)*t,y=a.y+(s*b.y-a.y)*t,z=a.z+(s*b.z-a.z)*t,w=a.w+(s*b.w-a.w)*t;const l=Math.hypot(x,y,z,w)||1;return {x:x/l,y:y/l,z:z/l,w:w/l};}
function frScript(c,dt,S,t){
  const b=ball;
  if(!S.fr){
    const b0={x:b.pos.x,y:b.pos.y,z:b.pos.z},w1={x:HX-BR,y:BR+4,z:b0.z+20},w2={x:HX-BR,y:H*.7,z:w1.z+34},c1={x:HX-34,y:H-BR,z:w2.z+14};
    S.fr={ft:0,legs:[[0,FR.TJ-3.4,b0,w1],[FR.TJ-3.4,FR.TJ,w1,w2],[FR.TJ,FR.TREL,w2,c1]],up:{x:0,y:1,z:0},d:{x:1,y:0,z:0},rel:false,air:false,tc:-1,ts:-1};
  }
  const F=S.fr;F.ft+=dt;const ft=F.ft,tmp={},qA={x:0,y:0,z:0,w:1};
  /* BALLE : chemin scripté sol → mur → plafond (figée), puis, posée sur le plafond, la vraie physique reprend (retombée lente) */
  if(!F.rel){
    let lg=F.legs[F.legs.length-1];for(const L of F.legs){if(ft<L[1]){lg=L;break;}}
    const k=Math.min(1,Math.max(0,(ft-lg[0])/(lg[1]-lg[0]))),a=lg[2],e=lg[3];
    b.pos.set(a.x+(e.x-a.x)*k,a.y+(e.y-a.y)*k,a.z+(e.z-a.z)*k);frProj(b.pos,BR);b.vel.set(0,0,0);b.mesh.position.copy(b.pos);
    F.leg=lg;
    if(ft>FR.TREL-.4)S.ceil=true;
    if(ft>=FR.TREL){F.rel=true;S.ceil=true;b.frozen=0;b.vel.set(-5,0,8);}
  }
  /* VOITURE */
  if(F.ts<0){
    if(ft<FR.TJ){
      /* 1) la voiture pousse la balle juste devant elle, collée au sol puis à la paroi, jusqu'en haut du mur */
      const lg=F.leg;let dx=lg[3].x-lg[2].x,dy=lg[3].y-lg[2].y,dz=lg[3].z-lg[2].z;const dl=Math.hypot(dx,dy,dz)||1;dx/=dl;dy/=dl;dz/=dl;
      const m6=Math.min(1,dt*6);F.d.x+=(dx-F.d.x)*m6;F.d.y+=(dy-F.d.y)*m6;F.d.z+=(dz-F.d.z)*m6;
      const D=5.6+Math.max(0,9-9*ft),px={x:b.pos.x-F.d.x*D,y:b.pos.y-F.d.y*D,z:b.pos.z-F.d.z*D};
      frProj(px,CR);sdf(px.x,px.y,px.z,FR.S,false);
      const m8=Math.min(1,dt*8);F.up.x+=(-FR.S.nx-F.up.x)*m8;F.up.y+=(-FR.S.ny-F.up.y)*m8;F.up.z+=(-FR.S.nz-F.up.z)*m8;
      const ul=Math.hypot(F.up.x,F.up.y,F.up.z)||1;F.up.x/=ul;F.up.y/=ul;F.up.z/=ul;
      frBasis(F.up,F.d,tmp);frQuat(qA,tmp.r,F.up,tmp.b);
      const ox=c.pos.x,oy=c.pos.y,oz=c.pos.z;frApply(c,px.x,px.y,px.z,qA);
      c.vel.set((px.x-ox)/dt,(px.y-oy)/dt,(px.z-oz)/dt);c.boosting=true;c.grounded=true;
      return;
    }
    if(!F.air){ /* en haut du mur : la voiture décolle (on garde sa position / son orientation de départ) */
      F.air=true;F.pj={x:c.pos.x,y:c.pos.y,z:c.pos.z};F.qj={x:c.q.x,y:c.q.y,z:c.q.z,w:c.q.w};
      const gx=-b.pos.x,gz=HZ-b.pos.z,gl=Math.hypot(gx,gz)||1;
      frBasis({x:0,y:1,z:0},{x:gx/gl,y:0,z:gz/gl},tmp);F.qu={x:0,y:0,z:0,w:1};frQuat(F.qu,tmp.r,{x:0,y:1,z:0},tmp.b);
    }
    /* 2) elle se remet à l'endroit, puis fait un DEMI AIR ROLL (180°) pour passer sous la balle, roues vers elle */
    const u=Math.min(1,(ft-FR.TJ)/(FR.TREL+FR.TAPP-FR.TJ));
    let q=frNlerp(F.qj,F.qu,frSS(0,.3,u));
    const th=Math.PI*frSS(.3,.88,u);q=frQmul(q,{x:0,y:0,z:Math.sin(th/2),w:Math.cos(th/2)});
    const e=frSS(0,1,u),gap=BR+.7+.12,tx=b.pos.x,ty=b.pos.y-gap,tz=b.pos.z;
    const ox=c.pos.x,oy=c.pos.y,oz=c.pos.z;
    frApply(c,F.pj.x+(tx-F.pj.x)*e,F.pj.y+(ty-F.pj.y)*e,F.pj.z+(tz-F.pj.z)*e,q);
    c.vel.set((c.pos.x-ox)/dt,(c.pos.y-oy)/dt,(c.pos.z-oz)/dt);c.boosting=u<.6;c.grounded=false;
    if(u>=1&&F.tc<0){ /* les 4 roues touchent la balle : FLIP RESET */
      F.tc=ft;flipReset(c);S.rs=t;S.msg='FLIP RESET !';burst(b.pos,0xffd166,16,12,.4);
    }
    if(F.tc>=0&&ft>F.tc+FR.TSHOT){ /* flip rendu : frappe dans le but (gravité réduite et traînée compensées) */
      F.ts=ft;const g=G*.3,sp=56,ddx=0-b.pos.x,ddy=GH*.4-b.pos.y,ddz=HZ-b.pos.z,dd=Math.hypot(ddx,ddy,ddz),T=dd/sp;
      b.vel.set(ddx/T*1.03,ddy/T+.5*g*T,ddz/T*1.03);
      burst(b.pos,0xffffff,18,20,.35);addShake(.3);F.qs={x:c.q.x,y:c.q.y,z:c.q.z,w:c.q.w};F.cv={x:c.vel.x,y:c.vel.y,z:c.vel.z};
    }
    return;
  }
  /* 3) après la frappe : la voiture finit son flip (rotation avant) et continue sur sa lancée, sans passer sous le sol */
  const v=F.cv,k=Math.min(1,(ft-F.ts)/.5),th=Math.PI*2*frSS(0,1,k),dm=Math.max(0,1-dt*1.2);
  v.x*=dm;v.z*=dm;v.y=v.y*dm-G*.3*dt;
  const q=frQmul(F.qs,{x:Math.sin(th/2),y:0,z:0,w:Math.cos(th/2)});
  frApply(c,c.pos.x+v.x*dt,Math.max(CR+.3,c.pos.y+v.y*dt),c.pos.z+v.z*dt,q);c.vel.set(v.x,v.y,v.z);c.boosting=false;
}
/*FRS-END*/
const MOVES=[
{id:'dodge',n:'Flip (dodge)',ic:'🌀',lvl:'Facile',lim:9,after:1.2,
 def:'Tu sautes, puis tu ré-appuies sur saut dans la seconde et demie qui suit en poussant une direction : la voiture fait une flip dans cette direction et gagne un gros coup de vitesse. C\'est la base de presque tous les autres moves.',
 steps:['Roule vers la balle avec le boost.','Appuie sur <b>Espace</b> pour sauter.','Ré-appuie sur <b>Espace</b> en poussant <b>↑</b> : flip avant dans la balle.'],
 goal:'Frappe la balle avec une flip.',
 setup(c,demo){mvPlace(c,demo?0:JIT()*16,CR+.02,-30,0,0);mvBall(demo?0:JIT()*10,BR,10,0,0,0,false);},
 demo(c,t,I,S){
   const d=ball.pos.z-c.pos.z;I.thr=1;I.boost=!S.j;
   if(S.j===undefined&&c.grounded&&d<19)S.j=t;
   if(S.j!==undefined){
     if(t-S.j<.1)I.jump=true;
     else if(t-S.j>.16&&t-S.j<.22){I.y=-1;I.jump=true;}
   }
 },
 touch(c,i,S){ if(c.flipT>0)return true; S.msg='Sans flip… saute puis ré-appuie sur Espace !';return false; }
},
{id:'halfflip',n:'Half-flip',ic:'↩️',lvl:'Moyen',lim:6,after:1.2,
 def:'Un flip arrière suivi d\'un air roll : la voiture se retourne de 180° en moins d\'une seconde. Tu repars dans la direction opposée en gardant ton élan, sans faire un grand demi-tour.',
 steps:['Saute avec <b>Espace</b>.','Ré-appuie sur <b>Espace</b> en tirant <b>↓</b> (flip arrière).','Quand la voiture se retrouve nez vers l\'arrière, pousse <b>↑</b> pour annuler la rotation, puis fais un air roll (<b>Q/E</b>) pour retomber sur les roues.','Boost !'],
 goal:'Fais un half-flip : flip arrière, retourne-toi et atteins la vitesse de 25 face à la balle.',
 setup(c,demo){mvPlace(c,0,CR+.02,0,0,0);mvBall(0,BR,-50,0,0,0,false);},
 demo(c,t,I,S){
   if(t<.1)I.jump=true;
   if(H_(t,.12,.17)){I.y=1;I.jump=true;if(S.f===undefined)S.f=t;}
   if(S.f!==undefined&&t>S.f+.05){
     if(c.flipT>0&&c.fwd.z>-.6&&!S.c)I.y=0;
     if(c.fwd.z<=-.6)S.c=true;
     if(S.c&&c.flipT>0)I.y=-1;
     if(S.c&&!c.grounded)I.roll=(c.up.y<-.2&&Math.abs(c.right.y)<.25)?1:clamp(c.right.y*3,-1,1);
     I.boost=true;
   }
   if(c.grounded&&t>.4){I.thr=1;I.boost=true;}
 },
 tick(c,dt,S,t){
   if(c.dodgeAge<.08&&c.flipF<-.5)S.back=true;
   if(S.back&&c.grounded&&c.fwd.z<-.8&&-c.vel.z>=25)return true;
   if(t>4.5&&!S.back){S.msg='Il faut un flip arrière (Espace ×2 en tirant ↓)';return false;}
 }
},
{id:'speedflip',n:'Speed flip',ic:'⚡',lvl:'Difficile',lim:9,after:1.2,
 def:'Un flip diagonal que tu annules presque aussitôt, puis tu fais un air roll pour retomber à plat : tu gardes (et gagnes) de la vitesse sans perdre de temps en l\'air. C\'est le move du coup d\'envoi.',
 steps:['Boost + avance (<b>↑</b> + <b>Maj</b>).','Saute, puis flip <b>diagonal</b> : <b>Espace</b> avec <b>↑</b> + <b>←</b> ou <b>→</b>.','Tout de suite après, pousse la direction opposée (<b>↓</b> + sens inverse) pour <b>annuler</b> la rotation.','Air roll pour retomber sur les roues, et garde le boost.'],
 goal:'Flip diagonal annulé + atterrissage à plat (reste en boost).',
 setup(c,demo){mvPlace(c,0,CR+.02,-52,0,0);mvBall(0,BR,0,0,0,0,false);},
 demo(c,t,I,S){
   I.thr=1;I.boost=true;
   if(t>=.6&&t<.63)I.jump=true;
   if(H_(t,.67,.71)){I.jump=true;I.y=-1;I.x=1;if(S.f===undefined)S.f=t;}
   if(S.f!==undefined&&t>S.f+.03&&c.flipT>0){I.y=1;I.x=-1;}
   if(S.f!==undefined&&t>S.f+.08&&!c.grounded&&c.flipT<=0){I.roll=clamp(c.right.y*3,-1,1);I.y=(c.fwd.y>.03)?-1:(c.fwd.y<-.03?1:0);}
 },
 tick(c,dt,S,t){
   if(!S.diag&&c.dodgeAge<.06&&c.flipF>.3&&Math.abs(c.flipR)>.3){S.diag=true;S.can=false;S.pre=c.vel.length();}
   if(S.diag&&c.dodgeAge<1&&c.flipT>0&&c.flipRate<6)S.can=true;
   if(!S.diag&&c.dodgeAge<.06&&!(c.flipF>.3&&Math.abs(c.flipR)>.3)){S.msg='Il faut une flip diagonale (↑ + ←/→)';return false;}
 },
 land(c,i,S){
   if(!S.diag)return;
   if(!S.can){S.msg='Annule la flip (direction opposée) avant de retomber';return false;}
   if(c.up.y<.7){S.msg='Retombe sur les roues (air roll)';return false;}
   S.msg='Speed flip propre ! '+Math.round(c.vel.length()*2.7)+' km/h';return true;
 }
},
{id:'wavedash',n:'Wave dash',ic:'🌊',lvl:'Difficile',lim:12,after:1.2,
 def:'Après un petit saut, tu fais un dodge en piquant du nez pour que la voiture retombe presque tout de suite : la flip est « convertie » en vitesse au sol. Pratique pour garder de la vitesse quand tu n\'as plus de boost.',
 steps:['Roule à bonne vitesse.','Un petit saut : tape <b>Espace</b>.','Pique du nez (<b>↑</b>) pendant la descente.','Juste avant de toucher le sol, ré-appuie sur <b>Espace</b> avec <b>↑</b> : atterris pendant la flip.'],
 goal:'Atterris en pleine flip, à plat, en gagnant de la vitesse.',
 setup(c,demo){mvPlace(c,demo?0:JIT()*10,CR+.02,-45,0,24);mvBall(0,BR,50,0,0,0,false);},
 demo(c,t,I,S){I.thr=1;if(t>=.15&&t<.18)I.jump=true;if(t>=.45&&t<.51)I.y=-1;if(H_(t,.77,.81)){I.jump=true;I.y=-1;}},
 tick(c,dt,S,t){
   if(c.grounded&&!S.pend)S.v0=c.vel.length();
   if(S.pend&&t>S.pend.t+.35){
     if(c.grounded&&c.up.y>.9){S.msg='Wave dash ! '+Math.round(c.vel.length()*2.7)+' km/h (avant le saut : '+Math.round((S.v0||0)*2.7)+')';return true;}
     S.msg='Retombe à plat : pique du nez AVANT la flip';return false;
   }
 },
 land(c,i,S,t){
   if(S.pend||c.dodgeAge>=2)return;
   if(c.dodgeAge>.4){S.msg='Trop haut : dodge plus près du sol';return false;}
   S.pend={t:t};
 }
},
{id:'airroll',n:'Air roll',ic:'🔄',lvl:'Facile',lim:9,after:1,
 def:'Faire pivoter la voiture sur elle-même en l\'air (roulis) pour orienter ses roues ou son toit vers ce que tu veux : indispensable pour les aériens, les flicks, les resets et pour retomber sur les roues.',
 steps:['Saute (maintiens <b>Espace</b>), puis re-saute <u>sans direction</u> pour monter plus haut.','Maintiens <b>E</b> (ou <b>Q</b>), ou <b>Ctrl/X + ←/→</b>, pour faire rouler la voiture.','Fais un tour complet puis retombe sur les roues.'],
 goal:'Fais un tour complet (360°) en l\'air.',
 setup(c,demo){mvPlace(c,0,CR+.02,-20,0,22);mvBall(0,BR,50,0,0,0,false);},
 demo(c,t,I,S){
   I.thr=1;
   if(t<.2)I.jump=true;
   else if(H_(t,.24,.3))I.jump=true;
   if(t>.3&&Math.abs(S.roll||0)<Math.PI*2*.97)I.roll=1;
 },
 tick(c,dt,S){
   if(!c.grounded){S.air=true;if(c.flipT<=0)S.roll=(S.roll||0)+c.av.dot(c.fwd)*dt;if(Math.abs(S.roll)>=Math.PI*2*.93){S.msg='Tour complet !';return true;}}
 },
 land(c,i,S){if(S.air){S.msg='Il faut un tour complet avant de retomber (maintiens E / Q)';return false;}}
},
{id:'fastaerial',n:'Aérien rapide',ic:'🚀',lvl:'Moyen',lim:6,after:1.2,
 def:'Un aérien optimisé : tu sautes en inclinant le nez vers le haut avec le boost, puis tu fais un second saut sans direction pendant que tu montes. Tu atteins la balle bien plus vite qu\'avec un simple saut.',
 steps:['Saute (<b>Espace</b>) en tirant <b>↓</b> et en appuyant sur le boost.','Un instant plus tard, ré-appuie sur <b>Espace</b> <u>sans</u> pousser de direction (second saut).','Oriente-toi vers la balle avec <b>↑↓←→</b> et garde le boost.'],
 goal:'Touche la balle en l\'air (plus de 9 de haut) en moins de 3,5 s.',
 setup(c,demo){mvPlace(c,demo?0:JIT()*14,CR+.02,-8,0,0);mvBall(demo?0:JIT()*14,15,18,0,0,0,true);},
 demo(c,t,I,S){
   if(t<.2){I.jump=true;I.y=1;I.boost=t>.03;}
   else if(t<.24){I.y=1;I.boost=true;}
   else if(t<.3){I.jump=true;I.boost=true;}
   else{const r=mvAim(c,I,ball.pos.x,ball.pos.y,ball.pos.z);I.boost=r.lf>.8;}
 },
 touch(c,i,S,t){
   if(i.air&&ball.pos.y>9){S.msg='Aérien en '+t.toFixed(1)+' s';return t<=3.5?true:false;}
   S.msg='Touche la balle en l\'air';return false;
 }
},
{id:'flipreset',n:'Flip reset',ic:'♻️',lvl:'Expert',lim:40,after:2.4,ceilG:.3,
 def:'Le flip reset complet, jusqu\'au but, entièrement à toi : tu amènes la balle sur le mur, tu la fais rouler le long du mur, puis tu l\'envoies atterrir sur le plafond. Là, elle retombe lentement : tu la rattrapes, tu fais un demi air roll (180°) pour te retourner et tu la touches avec les quatre roues, donc le dessous de la voiture. Le jeu croit que tu as retouché le sol et te rend ton flip (double saut), sans limite de temps. Tu l\'utilises pour frapper la balle dans le but.',
 steps:['Tu es dans ton camp, avec la balle devant toi. <b>Amène-la sur le mur de droite</b> en la gardant près de toi (<b>boost</b>).','<b>Fais-la rouler sur le mur</b> : monte sur la paroi en la poussant devant toi.','Envoie-la sur le <b>plafond</b> : une fois dessus, sa retombée est <b>lente</b>.','Pendant qu\'elle retombe, saute du mur (<b>Espace</b>) et fais un <b>demi air roll</b> (<b>Q</b> ou <b>E</b>, 180°) : le dessous de la voiture, avec les 4 roues, doit être face à la balle.','Coupe le boost et touche la balle avec les 4 roues en même temps : <b>FLIP RESET</b> (ding, étincelles, pastille « FLIP DISPO »).','Ton flip est rendu : refais <b>Espace</b> + direction pour frapper la balle <b>dans le but adverse</b>.'],
 goal:'Amène la balle sur le mur, fais-la rouler, envoie-la au plafond, fais ton demi air roll pendant sa retombée lente : touche-la avec les 4 roues pour récupérer ton flip, puis marque dans le but adverse.',
 setup(c,demo){
   /* départ dans ton camp, balle juste devant la voiture, côté mur de droite */
   const j=demo?0:1,z=-HZ*.6+j*JIT()*8,cx=HX-34+j*JIT()*4,bx=cx+8,bz=z+12;
   mvPlace(c,cx,CR+.02,z,Math.atan2(bx-cx,bz-z),14);
   mvBall(bx,BR,bz,0,0,0,!!demo);   /* démo : la balle reste figée sur son chemin scripté (frScript) */
   if(demo)c.ghost=true;           /* démo : pas de physique de voiture, pilotée par frScript */
 },
 demo(c,t,I,S){},
 script(c,dt,S,t){frScript(c,dt,S,t);},
 touch(c,i,S,t){
   if(S.rs!==undefined)return undefined;   /* le reset est déjà fait : on frappe maintenant la balle vers le but */
   if(!i.air)return undefined;   /* balle amenée / roulée au sol ou sur le mur : on continue */
   if(i.w){
     S.rs=t;S.msg=i.rs?'FLIP RESET !':'FLIP RESET ! (en match, utilise d\'abord ton flip : un reset ne rend qu\'un flip déjà dépensé)';
     return undefined;   /* à toi de finir dans le but */
   }
   if(i.ny>.5){S.msg='Tu l\'as touchée avec le toit : fais un demi air roll (Q ou E) pour te retourner, roues vers la balle';return false;}
   if(i.ny<-.3){S.msg='Presque ! Présente tout le dessous (les 4 roues) à plat face à la balle, pas juste un bord';return false;}
   S.msg='Touche la balle avec le dessous de la voiture (les 4 roues) : demi air roll (Q ou E)';return false;
 },
 tick(c,dt,S,t){
   if(!S.ceil&&ball.pos.y>H-BR-1.5)S.ceil=true;   /* la balle a touché le plafond : sa retombée devient lente (ceilG) */
   if(S.rs!==undefined&&MV.state==='try'&&t>S.rs+6){S.msg='Flip reset réussi, mais la balle n\'est pas rentrée dans le but adverse';return false;}
 },
 goal_(c,team,S){
   if(team===0&&S.rs!==undefined){S.msg='Flip reset dans le but !';return true;}
   S.msg=team===0?'But marqué, mais sans flip reset':'Mauvais but';return false;
 }
},
{id:'pinch',n:'Ground pinch (au sol)',ic:'🤏',lvl:'Difficile',lim:10,after:1.6,
 def:'Le « ground pinch » : tu coinces la balle entre ta voiture et le sol : elle est écrasée puis éjectée à une vitesse bien plus grande que celle d\'une frappe normale (jusqu\'à environ le double de ta vitesse). C\'est le tir surprise des pros, très dur à arrêter pour le gardien.',
 steps:['Roule vers la balle avec le boost : elle est posée au sol.','Saute <u>de loin</u> (environ une seconde avant d\'arriver sur la balle) avec <b>Espace</b>.','Une fois en l\'air, pique du nez (<b>↑</b>) et garde le <b>boost</b> : tu plonges sur le dessus de la balle.','Pose le dessous / l\'avant de la voiture sur la balle : elle est coincée contre le sol et part comme un boulet.'],
 goal:'Écrase la balle contre le sol avec le dessous de la voiture : elle doit repartir au moins 1,6 fois plus vite que ta voiture.',
 setup(c,demo){mvPlace(c,demo?0:JIT()*3,CR+.02,-28,0,22);mvBall(demo?0:JIT()*2,BR,12,0,0,0,false);},
 demo(c,t,I,S){
   const dz=ball.pos.z-c.pos.z;
   I.thr=1;I.boost=c.grounded&&c.vel.length()<24;
   if(S.j===undefined&&c.grounded&&dz<28)S.j=t;
   if(S.j!==undefined){
     if(t-S.j<.2)I.jump=true;
     else if(!c.grounded){mvAim(c,I,ball.pos.x,ball.pos.y+BR*.7,ball.pos.z);I.boost=c.vel.y<0;}
   }
 },
 touch(c,i,S,t){return pinchTouch(c,S,t,'sol');},
 tick(c,dt,S,t){return pinchTick(S,t,{minSp:30,minR:1.6,miss:'Pas de pinch : passe au-dessus de la balle et coince-la contre le sol',weak:'Plonge plus fort sur la balle (↑ + boost)'});}
},
{id:'pinchwall',n:'Wall pinch (au mur)',ic:'🧱',lvl:'Expert',lim:10,after:1.6,
 def:'Le « wall pinch » : même principe que le ground pinch, mais avec une paroi : la balle est collée au mur et tu l\'écrases avec l\'avant de ta voiture, en arrivant de biais. Coincée entre la voiture et le mur, elle est éjectée le long de la paroi bien plus vite que ta voiture.',
 steps:['La balle est collée au mur, à mi-hauteur. Roule vers elle <u>en diagonale</u> (pas pile face au mur) avec le boost.','Saute de loin avec <b>Espace</b>, puis incline le nez vers la balle (<b>↓</b>) en gardant le <b>boost</b>.','Touche la balle avec l\'avant de la voiture, <u>du côté intérieur</u> de l\'arène : elle se retrouve coincée entre toi et la paroi.','Elle est écrasée contre le mur et part le long de la paroi comme un boulet.'],
 goal:'Écrase la balle contre le mur : elle doit repartir au moins 1,4 fois plus vite que ta voiture.',
 setup(c,demo){
   const bx=HX-BR-.03,by=Math.max(R+BR+1,HY-1),bz=demo?0:JIT()*6;
   const cx=Math.max(-HX+8,HX-24)+(demo?0:JIT()*6),cz=-26+(demo?0:JIT()*4);
   mvPlace(c,cx,CR+.02,cz,Math.atan2(bx-cx,bz-cz),22);
   mvBall(bx,by,bz,0,0,0,true);
 },
 demo(c,t,I,S){
   if(t<.2){I.jump=true;I.y=1;I.boost=t>.03;}
   else if(t<.24){I.y=1;I.boost=true;}
   else if(t<.3){I.jump=true;I.boost=true;}
   else{const r=mvAim(c,I,ball.pos.x,ball.pos.y,ball.pos.z);I.boost=r.lf>.8;}
 },
 touch(c,i,S,t){return pinchTouch(c,S,t,'mur');},
 tick(c,dt,S,t){return pinchTick(S,t,{minSp:28,minR:1.4,miss:'Pas de pinch : arrive de biais et touche la balle du côté intérieur pour la coincer contre le mur',weak:'Arrive plus vite et de biais, nez vers la balle (boost)'});}
},
{id:'pinchroof',n:'Ceiling pinch (plafond → but)',ic:'🎯',lvl:'Expert',lim:14,after:1.8,
 def:'Le « ceiling pinch » : un enchaînement complet : tu roules sur le mur, tu montes jusqu\'au plafond, puis tu te laisses retomber sur la balle posée au sol et tu l\'écrases contre le sol. Avec la hauteur, ta voiture arrive très vite : le pinch envoie la balle directement dans le but.',
 steps:['Tu es sur le <b>mur</b>, la balle est au sol. Monte le long de la paroi avec le <b>boost</b> jusqu\'au <b>plafond</b>.','Sur le plafond, oriente-toi vers le but puis saute avec <b>Espace</b> pour te décoller.','Pique du nez vers la balle (<b>↑</b>) en gardant le <b>boost</b> : tu redescends dessus, la balle entre toi et le but.','Écrase la balle contre le sol : le pinch l\'envoie comme un boulet, droit dans la cage. Ne plonge pas trop à pic (50° maximum) : sinon la balle rebondit sur le sol et passe au-dessus de la barre.'],
 goal:'Roule du mur au plafond, redescends sur la balle et fais un pinch au sol qui l\'envoie directement dans le but.',
 setup(c,demo){
   mvPlaceWall(c,1,9+(demo?0:JIT()*2),-30+(demo?0:JIT()*4),24,.75);
   mvBall(6+(demo?0:JIT()*3),BR,44+(demo?0:JIT()*3),0,0,0,false);   /* z=44 (et non 24) : calculé par simulation, la plongée peut aller jusqu'à ≈ 50° sans que la balle rebondisse au-dessus de la barre */
   if(demo)c.ghost=true;   /* démo : voiture pilotée par pcScript, trajectoire de la balle calculée (pnVel) */
 },
 demo(c,t,I,S){},
 script(c,dt,S,t){pcScript(c,dt,S,t);},
 touch(c,i,S,t){return pinchTouch(c,S,t,'sol');},
 tick(c,dt,S,t){return pinchTick(S,t,{goal:true,goalT:4.5,minSp:22,minR:1.25,miss:'Pas de pinch : redescends sur la balle et coince-la contre le sol',weak:'Redescends plus vite et plonge plus fort sur la balle (↑ + boost)',noGoal:'Bon pinch, mais la balle n\'est pas rentrée dans le but : place-toi sur la ligne balle → but avant de plonger, et ne plonge pas trop à pic (50° max : sinon elle rebondit au-dessus de la barre)'});},
 goal_(c,team,S){
   if(team===0&&S.pin){S.msg='Pinch dans le but !';return true;}
   S.msg=team===0?'But marqué, mais sans pinch':'Mauvais but';return false;
 }
},
{id:'pinchwallgoal',n:'Kuxir pinch (mur → but)',ic:'🧱',lvl:'Expert',lim:12,after:1.8,
 def:'Le « kuxir pinch » (du nom du joueur Kuxir, Francesco Cinquemani), le plus célèbre des pinchs : il envoie la balle de ton camp directement dans le but adverse. La balle est collée au mur de droite, dans ton camp. Tu fonces dessus en visant franchement la paroi et tu la coinces : elle est éjectée, rebondit sur le mur et traverse tout le terrain pour rentrer dans le but adverse.',
 steps:['La balle est collée au <b>mur de droite</b>, dans ton camp, à mi-hauteur. Tu démarres en l\'air, côté intérieur de l\'arène.','Fonce dessus avec le <b>boost</b>, <u>à l\'horizontale</u> (nez à ±5°), en arrivant à environ <b>55-60° du mur</b> : ni face à la paroi, ni en la longeant. Trop incliné vers le bas, la balle rebondit sur le sol et passe au-dessus de la barre.','Touche la balle avec l\'avant de ta voiture : elle est coincée entre toi et la paroi.','Elle rebondit sur le mur, file vers l\'autre bout du terrain et rentre dans la cage.'],
 goal:'Écrase la balle contre le mur : elle doit traverser le terrain et entrer directement dans le but adverse.',
 setup(c,demo){
   const bx=HX-BR-.03,by=9+(demo?0:JIT()*1.5),bz=-40+(demo?0:JIT()*4);
   const a=PK_A,d=20+(demo?0:JIT()*3);   /* 57° : centre de la fenêtre d'angles (49°-65°) qui marche, calculée par simulation */
   const cx=bx-Math.sin(a)*d+(demo?0:JIT()*3),cz=bz-Math.cos(a)*d,cy=by+3+(demo?0:JIT()*2);
   mvPlace(c,cx,cy,cz,Math.atan2(bx-cx,bz-cz),24);
   mvBall(bx,by,bz,0,0,0,true);
   if(demo)c.ghost=true;   /* démo : voiture pilotée par pkScript, trajectoire de la balle calculée (pnVel) */
 },
 demo(c,t,I,S){},
 script(c,dt,S,t){pkScript(c,dt,S,t);},
 touch(c,i,S,t){return pinchTouch(c,S,t,'mur');},
 tick(c,dt,S,t){return pinchTick(S,t,{goal:true,goalT:5,minSp:22,minR:1.25,miss:'Pas de pinch : fonce vers le mur, à peu près à l\'horizontale, pour coincer la balle contre la paroi',weak:'Arrive plus vite, nez vers la balle (boost)',noGoal:'Bon pinch, mais la balle n\'est pas rentrée dans le but : arrive à environ 55-60° du mur (49° à 65° marchent), à l\'horizontale (nez à ±5°)'});},
 goal_(c,team,S){
   if(team===0&&S.pin){S.msg='Pinch dans le but !';return true;}
   S.msg=team===0?'But marqué, mais sans pinch':'Mauvais but';return false;
 }
},
{id:'doubletap',n:'Double tap',ic:'✌️',lvl:'Expert',lim:30,after:2,
 def:'Le double tap (aussi appelé « double touch ») : tu frappes la balle contre le tableau arrière, le mur derrière le but, puis tu la retouches en l\'air au rebond pour la mettre dans la cage. Le gardien se jette sur la 1re touche et n\'a plus le temps de se replacer pour la 2e.',
 steps:['La balle flotte devant toi. Démarre avec le <b>boost</b> et fais un <b>aérien rapide</b> vers elle.','1re touche : frappe-la <u>vers le haut et le fond du terrain</u>, pour qu\'elle tape le <b>tableau arrière</b> <u>au-dessus</u> de la cage (pas dedans).','Ne coupe pas ton élan : <b>suis la balle</b> en boostant et prépare ton angle avec un air roll (<b>Q</b> / <b>E</b>).','Après le rebond, place-toi sous / derrière la balle, face au but : un petit coup doux marche mieux qu\'une frappe à fond.','2e touche, <u>en l\'air</u> : envoie-la dans le but.'],
 goal:'Frappe la balle sur le tableau arrière (au-dessus du but), puis reprends-la en l\'air au rebond et marque.',
 setup(c,demo){
   const j=demo?0:1;
   mvPlace(c,j*JIT()*6,CR+.02,HZ-52,0,18);
   mvBall(j*JIT()*4,12+j*JIT()*2,HZ-27+j*JIT()*3,0,0,0,true);   /* balle en l'air, figée jusqu'à la 1re touche */
   if(demo)c.ghost=true;   /* démo : voiture pilotée par dtScript (la balle garde la vraie physique, rebond compris) */
 },
 demo(c,t,I,S){},
 script(c,dt,S,t){dtScript(c,dt,S,t);},
 touch(c,i,S,t){
   if(S.t2!==undefined)return undefined;   /* la 2e touche est faite : à la balle d'aller dans le but */
   if(S.bb===undefined){   /* avant le rebond : c'est la 1re touche (on peut la refaire tant que la balle n'a pas touché le tableau) */
     if(!i.air){S.msg='Fais la 1re touche en l\'air : aérien rapide vers la balle';return false;}
     S.t1=t;return undefined;
   }
   if(!i.air){S.msg='2e touche au sol : reprends la balle en l\'air, au rebond';return false;}
   S.t2=t;S.msg='2e touche ! Maintenant, dans le but';return undefined;
 },
 tick(c,dt,S,t){
   if(S.t1!==undefined&&S.bb===undefined){
     if(ball.frozen<=0&&ball.pos.z>HZ-BR-.9&&(ball.pos.y>GH+BR*.4||Math.abs(ball.pos.x)>GW+BR*.4)){S.bb=t;return;}   /* la balle touche le tableau arrière, hors de la cage */
     if(t>S.t1+3.4){S.msg='La balle n\'a pas rebondi sur le tableau arrière : frappe plus fort, vers le haut et le fond, au-dessus de la cage';return false;}
   }
   if(S.bb!==undefined&&S.t2===undefined){
     if(ball.pos.y<BR+.7){S.msg='La balle a touché le sol : reprends-la en l\'air, juste après le rebond';return false;}
     if(t>S.bb+4){S.msg='Trop lent : reprends la balle en l\'air juste après le rebond';return false;}
   }
   if(S.t2!==undefined&&t>S.t2+3.6){S.msg='2e touche faite, mais la balle n\'est pas rentrée dans le but';return false;}
 },
 goal_(c,team,S){
   if(team===0&&S.t2!==undefined){S.msg='Double tap !';return true;}
   S.msg=team===0?(S.t1===undefined?'But marqué, mais sans double tap':'But marqué, mais sans rebond sur le tableau arrière + 2e touche'):'Mauvais but';return false;
 }
},
{id:'psycho',n:'Psycho (mur de ta cage)',ic:'🌀',lvl:'Expert',lim:20,after:2,
 def:'Le « psycho » : un tir où la balle tape d\'abord contre le mur derrière ta propre cage, dans ton camp, puis ressort et file jusqu\'au but adverse. Il porte le nom du joueur Owen « Psycho » Williams, dont le but a été élu but de l\'année 2019 par Gif Your Game. Le gardien adverse ne s\'attend pas à un tir qui vient de l\'autre bout du terrain, après un rebond.',
 steps:['La balle flotte en l\'air dans <b>ton camp</b>, devant ta cage, et tu regardes vers le fond du terrain. Le but : l\'envoyer taper contre le <b>mur derrière ta propre cage</b> pour qu\'elle <u>ressorte jusqu\'au but adverse</u>.','Fonce vers la balle avec le <b>boost</b> et fais un <b>aérien rapide</b>.','Frappe-la <u>franchement, tout droit vers le fond</u> (vers le mur derrière ta cage), avec un petit angle vers le haut : elle doit taper le mur <u>au-dessus de la barre de ta cage</u>. Attention : si elle passe sous la barre, elle rentre dans TA cage !','Le mur la renvoie en perdant de la vitesse : elle repart au-dessus du terrain, puis retombe.','Elle doit rentrer dans le but adverse.'],
 goal:'Frappe la balle contre le mur derrière ta cage pour qu\'elle ressorte et rentre dans le but adverse.',
 setup(c,demo){
   const j=demo?0:1;
   mvPlace(c,j*JIT()*3,CR+.02,-14,Math.PI,18);
   mvBall(j*JIT()*1.5,12+j*JIT()*1,-36+j*JIT()*1.5,0,0,0,true);   /* balle figée en l'air dans ton camp, jusqu'à la 1re touche */
   if(demo)c.ghost=true;
 },
 demo(c,t,I,S){},
 script(c,dt,S,t){psScript(c,dt,S,t);},
 touch(c,i,S,t){
   if(S.wall===undefined)S.t1=t;   /* dernière touche avant le rebond sur le mur */
   return undefined;
 },
 tick(c,dt,S,t){
   if(S.t1!==undefined&&S.wall===undefined&&ball.frozen<=0){
     if(ball.pos.z<-(HZ-BR-.9)&&!(Math.abs(ball.pos.x)<GW&&ball.pos.y<GH)){S.wall=t;S.msg='Rebond sur le mur de ta cage ! Elle repart vers l\'autre but';}
     else if(t>S.t1+3.2){S.msg='La balle n\'a pas touché le mur derrière ta cage : frappe-la tout droit vers le fond de ton camp';return false;}
   }
   if(S.wall!==undefined&&t>S.wall+6){S.msg='Rebond fait, mais la balle n\'est pas rentrée dans le but adverse';return false;}
 },
 goal_(c,team,S){
   if(team===0&&S.wall!==undefined){S.msg='Psycho !';return true;}
   if(team===0){S.msg='But marqué, mais sans rebond sur le mur derrière ta cage';return false;}
   S.msg=S.t1!==undefined&&S.wall===undefined?'Dans ta cage : vise le mur AU-DESSUS de la barre, pas la cage':'Mauvais but';return false;
 }
},
{id:'airdribble',n:'Air dribble',ic:'🎈',lvl:'Expert',lim:30,after:2,
 def:'L\'air dribble : tu gardes la balle en l\'air en la retouchant plusieurs fois avec ta voiture (petites touches + petits coups de boost), tout en la guidant vers le but adverse. Il se démarre depuis un mur (« wall air dribble » : tu montes la balle le long du mur et tu la décolles juste après la courbe) ou après un rebond sur le sol (« bounce air dribble », celui de cet exercice). La 1re touche est la plus importante.',
 steps:['La balle rebondit devant toi. Roule vers elle avec un peu de <b>boost</b>, à peu près à sa vitesse.','<b>Juste après un rebond</b>, frappe-la <u>par en dessous</u> et <b>saute au même moment</b> : elle monte, et toi tu restes juste derrière elle.','Une fois en l\'air, mets ta voiture <u>à plat derrière la balle</u>, nez vers elle, avec <b>↑ / ↓</b> et un air roll (<b>Q</b> / <b>E</b>).','Garde-la avec de <b>petites touches</b> : boost par petites pressions (pas à fond), toujours en la touchant <u>par en dessous</u> pour qu\'elle reste au-dessus de ta voiture.','Il te faut <b>3 touches en l\'air d\'affilée</b> (moins de 1,8 s entre deux) ; ensuite, mets-la dans le but.'],
 goal:'Garde la balle en l\'air avec au moins 3 touches d\'affilée, puis marque.',
 setup(c,demo){
   const j=demo?0:1;
   mvPlace(c,j*JIT()*4,CR+.02,-34,0,24);
   if(demo){mvBall(0,BR,-8,0,0,0,true);c.ghost=true;}   /* démo : balle posée au sol, figée jusqu'à la 1re touche du coach (adScript) */
   else mvBall(j*JIT()*2,6,-8+j*JIT()*2,0,0,8,false);   /* à toi : la balle tombe et rebondit devant toi */
 },
 demo(c,t,I,S){},
 script(c,dt,S,t){adScript(c,dt,S,t);},
 touch(c,i,S,t){
   const gap=S.cT===undefined?9:t-S.cT;S.cT=t;
   if(gap<.06)return undefined;   /* même contact (plusieurs images d'affilée) */
   if(i.air){
     S.air=(S.air||0)+1;
     if(S.air<=3)showMsg(S.air+' / 3','touches en l\'air d\'affilée','#7dffb0',.6);
   }else if((S.air||0)>0&&S.air<3)S.air=0;   /* retouchée au sol avant d'avoir 3 touches : la chaîne repart de zéro */
   return undefined;
 },
 tick(c,dt,S,t){
   const a=S.air||0;
   if(a>=1&&a<3){
     if(ball.pos.y<BR+.7&&t>S.cT+.15){S.msg='La balle est retombée au sol : il faut 3 touches en l\'air d\'affilée';return false;}
     if(t>S.cT+1.8){S.msg='Trop long entre deux touches : reste collé derrière la balle';return false;}
   }
   if(a>=3&&t>S.cT+4.5){S.msg='3 touches en l\'air faites, mais la balle n\'est pas entrée dans le but';return false;}
 },
 goal_(c,team,S){
   const a=S.air||0;
   if(team===0&&a>=3){S.msg='Air dribble !';return true;}
   S.msg=team===0?'But marqué, mais seulement '+a+' touche'+(a>1?'s':'')+' en l\'air d\'affilée (il en faut 3)':'Mauvais but';return false;
 }
}

];

/* DÉMO SCRIPTÉE du psycho : la voiture du coach est pilotée image par image jusqu'à la balle figée, qu'elle frappe vers le mur derrière sa cage ;
   la balle garde ensuite la vraie physique (rebond sur le mur compris). La vitesse du coup (PS_V) a été calculée avec une simulation de la physique de la balle
   (gravité, frottement de l'air, rebond du mur qui la ralentit) : le rebond la renvoie au-dessus de la barre adverse puis dans le but adverse. */
const PS_V=[0,12.53,-89.12];
function psScript(c,dt,S,t){
  const b=ball,hz=c.hb?c.hb.hz:2.15,gap=BR+hz+.15,tmp={},qA={x:0,y:0,z:0,w:1};
  if(!S.ps)S.ps={ft:0,st:0,p0:{x:c.pos.x,y:c.pos.y,z:c.pos.z},f0:{x:c.fwd.x,y:c.fwd.y,z:c.fwd.z},v:{x:0,y:0,z:0},TM:1.6};
  const F=S.ps;F.ft+=dt;
  const pose=(px,py,pz,fx,fy,fz)=>{
    const fl=Math.hypot(fx,fy,fz)||1,fwd={x:fx/fl,y:fy/fl,z:fz/fl},ud=fwd.y;   /* up = haut du monde, rendu perpendiculaire à l'avant */
    const wx=-fwd.x*ud,wy=1-fwd.y*ud,wz=-fwd.z*ud,ul=Math.hypot(wx,wy,wz)||1,up={x:wx/ul,y:wy/ul,z:wz/ul};
    frBasis(up,fwd,tmp);frQuat(qA,tmp.r,up,tmp.b);
    const ox=c.pos.x,oy=c.pos.y,oz=c.pos.z;frApply(c,px,py,pz,qA);
    c.vel.set((px-ox)/dt,(py-oy)/dt,(pz-oz)/dt);F.v.x=c.vel.x;F.v.y=c.vel.y;F.v.z=c.vel.z;
  };
  const sp=Math.hypot(PS_V[0],PS_V[1],PS_V[2]),sd={x:PS_V[0]/sp,y:PS_V[1]/sp,z:PS_V[2]/sp};
  if(F.st===0){
    /* élan, saut et boost : la voiture monte à la balle, le nez vers le mur du fond */
    const u=Math.min(1,F.ft/F.TM),e=Math.pow(u,1.3),ey=Math.pow(u,2),k=frSS(.3,1,u);
    const tx=b.pos.x-sd.x*gap,ty=b.pos.y-sd.y*gap,tz=b.pos.z-sd.z*gap;
    pose(F.p0.x+(tx-F.p0.x)*e,F.p0.y+(ty-F.p0.y)*ey,F.p0.z+(tz-F.p0.z)*e,F.f0.x+(sd.x-F.f0.x)*k,.35+(sd.y-.35)*k,F.f0.z+(sd.z-F.f0.z)*k);
    c.boosting=u>.1&&u<.97;c.grounded=u<.1;
    if(u>=1){   /* la frappe : la balle part contre le mur derrière la cage de ton camp */
      b.frozen=0;b.vel.set(PS_V[0],PS_V[1],PS_V[2]);
      S.t1=t;F.st=1;burst(b.pos,0xffffff,18,20,.35);addShake(.3);
    }
    return;
  }
  /* après la frappe : la voiture finit sa course sans toucher le mur */
  const dm=Math.max(0,1-dt*1.5),v=F.v;v.x*=dm;v.y=v.y*dm-G*.25*dt;v.z*=dm;
  pose(c.pos.x+v.x*dt,Math.max(CR+.3,c.pos.y+v.y*dt),Math.max(-(HZ-hz-2),c.pos.z+v.z*dt),c.fwd.x,c.fwd.y,c.fwd.z);
  c.boosting=false;
}

/* ---- PINCHS : trajectoires CALCULÉES (démos scriptées) ----
   pnVel : mêmes formules que carBall (impulsion Psyonix + éjection du pinch) pour une balle au repos touchée par une voiture qui arrive à la vitesse s le long de tB.
   n = normale de la surface contre laquelle la balle est écrasée (vers l'extérieur de l'arène). Les angles ci-dessous viennent d'une simulation de vol de balle
   (gravité, frottement de l'air, rebonds sol / murs) :
   - KUXIR : mur de droite, balle à (HX, 9, -40). Le but marche pour un angle d'approche de 49° à 65° par rapport au mur (centre ≈ 57°), nez à l'horizontale (-6° à +5°) ;
     plus incliné vers le bas, la balle rebondit sur le sol et passe au-dessus de la barre.
   - CEILING : balle au sol à (6, z=44). Le but marche pour une plongée jusqu'à ≈ 50° (45° au centre) ; trop à pic, la balle rebondit sur le sol et passe au-dessus de la barre.
     (avec la balle à z=24, la limite n'était que de 38°). */
const PK_A=57*Math.PI/180;   /* angle d'approche du kuxir (centre de la fenêtre qui marche) */
const PC_TH=42*Math.PI/180;  /* plongée de la démo du ceiling pinch */
function pnVel(tB,s,n,mul){
  const rv=s,j=.8*rv,psc=rv<11.7?.65:rv<54?lerp(.65,.55,(rv-11.7)/42.3):lerp(.55,.3,Math.min(1,(rv-54)/54));
  let dx=tB.x,dy=tB.y*.35,dz=tB.z;const dl=Math.hypot(dx,dy,dz)||1;dx/=dl;dy/=dl;dz/=dl;
  let vx=tB.x*j*mul+dx*rv*psc*mul,vy=tB.y*j*mul+dy*rv*psc*mul,vz=tB.z*j*mul+dz*rv*psc*mul;
  const sn=tB.x*n.x+tB.y*n.y+tB.z*n.z,tx=tB.x-n.x*sn,ty=tB.y-n.y*sn,tz=tB.z-n.z*sn,tl=Math.hypot(tx,ty,tz);
  if(tl>.05){vx+=tx/tl*rv*mul;vy+=ty/tl*rv*mul;vz+=tz/tl*rv*mul;}
  vx-=n.x*rv*.12;vy-=n.y*rv*.12;vz-=n.z*rv*.12;
  const sp=Math.hypot(vx,vy,vz);if(sp>BMAX){const k=BMAX/sp;vx*=k;vy*=k;vz*=k;}
  return {x:vx,y:vy,z:vz};
}
/* pose d'une voiture pilotée à la main (démos scriptées) : up est rendu perpendiculaire à fwd, la vitesse est déduite du déplacement */
function pnPose(c,dt,F,tmp,qA,px,py,pz,f,u){
  const fl=Math.hypot(f.x,f.y,f.z)||1,fx=f.x/fl,fy=f.y/fl,fz=f.z/fl,ud=u.x*fx+u.y*fy+u.z*fz;
  const wx=u.x-fx*ud,wy=u.y-fy*ud,wz=u.z-fz*ud,ul=Math.hypot(wx,wy,wz)||1,up={x:wx/ul,y:wy/ul,z:wz/ul},fwd={x:fx,y:fy,z:fz};
  frBasis(up,fwd,tmp);frQuat(qA,tmp.r,up,tmp.b);
  const ox=c.pos.x,oy=c.pos.y,oz=c.pos.z;frApply(c,px,py,pz,qA);
  c.vel.set((px-ox)/dt,(py-oy)/dt,(pz-oz)/dt);F.v.x=c.vel.x;F.v.y=c.vel.y;F.v.z=c.vel.z;
}
/* DÉMO du kuxir pinch : la voiture arrive à l'horizontale, à 57° du mur, à ≈ 40 de vitesse ; au contact la balle reçoit la vitesse calculée par pnVel */
function pkScript(c,dt,S,t){
  const b=ball,hz=c.hb?c.hb.hz:2.15,gap=BR+hz+.15,tmp={},qA={x:0,y:0,z:0,w:1},tB={x:Math.sin(PK_A),y:0,z:Math.cos(PK_A)},up0={x:0,y:1,z:0};
  if(!S.pk){const C={x:b.pos.x-tB.x*gap,y:b.pos.y,z:b.pos.z-tB.z*gap};S.pk={ft:0,st:0,C:C,p0:{x:C.x-tB.x*20,y:C.y+2,z:C.z-tB.z*20},v:{x:0,y:0,z:0},TM:.7};}
  const F=S.pk;F.ft+=dt;
  if(F.st===0){
    const u=Math.min(1,F.ft/F.TM),e=Math.pow(u,1.2),ey=(1-u)*(1-u);
    pnPose(c,dt,F,tmp,qA,F.p0.x+(F.C.x-F.p0.x)*e,F.C.y+2*ey,F.p0.z+(F.C.z-F.p0.z)*e,{x:tB.x,y:-.08*(1-u),z:tB.z},up0);
    c.boosting=true;c.grounded=false;
    if(u>=1){
      const v=pnVel(tB,40,{x:1,y:0,z:0},1.08);
      b.frozen=0;b.vel.set(v.x,v.y,v.z);S.t1=t;S.pin={t:t,cs:40};F.st=1;burst(b.pos,0xffd166,22,18,.5);addShake(.3);
    }
    return;
  }
  const dm=Math.max(0,1-dt*3),v=F.v;v.x*=dm;v.y=v.y*dm-G*.3*dt;v.z*=dm;
  pnPose(c,dt,F,tmp,qA,Math.min(HX-hz-1,c.pos.x+v.x*dt),Math.max(CR+.3,c.pos.y+v.y*dt),c.pos.z+v.z*dt,{x:c.fwd.x,y:c.fwd.y,z:c.fwd.z},up0);
  c.boosting=false;
}
/* DÉMO du ceiling pinch : mur de droite → quart de cercle → plafond → saut → plongée (42°) sur la balle posée au sol, droit vers le but ; pnVel donne la vitesse de la balle */
function pcScript(c,dt,S,t){
  const b=ball,hz=c.hb?c.hb.hz:2.15,gap=BR+hz+.15,tmp={},qA={x:0,y:0,z:0,w:1};
  if(!S.pc){
    const gx=-b.pos.x,gz=HZ+4-b.pos.z,gl=Math.hypot(gx,gz)||1,g={x:gx/gl,z:gz/gl},cs=Math.cos(PC_TH),sn=Math.sin(PC_TH);
    const tB={x:g.x*cs,y:-sn,z:g.z*cs},C={x:b.pos.x-tB.x*gap,y:b.pos.y-tB.y*gap,z:b.pos.z-tB.z*gap};
    const yc=H-CR-.05,L=(yc-C.y)/sn,J={x:C.x-tB.x*L,y:yc,z:C.z-tB.z*L};   /* J : point du plafond où la voiture se décolle */
    const P0={x:c.pos.x,y:c.pos.y,z:c.pos.z},pit=.75,wl=(H-R-P0.y)/Math.sin(pit);
    const W={x:HX-CR-.05,y:H-R,z:P0.z+wl*Math.cos(pit)},E={x:HX-R,y:yc,z:W.z+6};   /* W : début du quart de cercle, E : sa fin (sur le plafond) */
    const h={x:J.x-E.x,y:0,z:J.z-E.z},hl=Math.hypot(h.x,h.z)||1;
    S.pc={ft:0,st:0,tB:tB,C:C,J:J,P0:P0,W:W,E:E,h:{x:h.x/hl,y:0,z:h.z/hl},pit:pit,v:{x:0,y:0,z:0},T:[.45,.3,Math.max(.7,hl/40),.65]};
  }
  const F=S.pc;F.ft+=dt;
  const T=F.T,t1=T[0],t2=t1+T[1],t3=t2+T[2],t4=t3+T[3];
  if(F.ft<t4){
    const ft=F.ft;
    if(ft<t1){   /* 1) on monte le long du mur */
      const u=ft/t1,sp=Math.sin(F.pit),cp=Math.cos(F.pit);
      pnPose(c,dt,F,tmp,qA,F.P0.x+(F.W.x-F.P0.x)*u,F.P0.y+(F.W.y-F.P0.y)*u,F.P0.z+(F.W.z-F.P0.z)*u,{x:0,y:sp,z:cp},{x:-1,y:0,z:0});
      c.boosting=true;c.grounded=true;
    }else if(ft<t2){   /* 2) quart de cercle mur → plafond (rayon de la courbe de l'arène) */
      const u=(ft-t1)/T[1],ph=u*Math.PI/2,rr=R-CR-.05+.0;
      pnPose(c,dt,F,tmp,qA,HX-R+rr*Math.cos(ph),H-R+rr*Math.sin(ph),F.W.z+(F.E.z-F.W.z)*u,{x:-Math.sin(ph)*.68,y:Math.cos(ph)*.68,z:.73},{x:-Math.cos(ph),y:-Math.sin(ph),z:0});
      c.boosting=true;c.grounded=true;
    }else if(ft<t3){   /* 3) sur le plafond, vers le point de décollage */
      const u=(ft-t2)/T[2];
      pnPose(c,dt,F,tmp,qA,F.E.x+(F.J.x-F.E.x)*u,F.E.y,F.E.z+(F.J.z-F.E.z)*u,F.h,{x:0,y:-1,z:0});
      c.boosting=true;c.grounded=true;
    }else{   /* 4) saut, retournement et plongée droit sur la balle, vers le but */
      const u=(ft-t3)/T[3],e=Math.pow(u,1.3),k=frSS(0,.65,u),tB=F.tB;
      const f={x:F.h.x+(tB.x-F.h.x)*k,y:tB.y*k,z:F.h.z+(tB.z-F.h.z)*k},fl=Math.hypot(f.x,f.y,f.z)||1;
      const u0y=1,ud=f.y/fl,sx=-f.x/fl*ud,sy=u0y-f.y/fl*ud,sz=-f.z/fl*ud,sl=Math.hypot(sx,sy,sz)||1;   /* U0 : « haut du monde » perpendiculaire à l'avant */
      const side={x:(f.y/fl*sz/sl-f.z/fl*sy/sl),y:(f.z/fl*sx/sl-f.x/fl*sz/sl),z:(f.x/fl*sy/sl-f.y/fl*sx/sl)};
      const rho=Math.PI*(1-k),cr=Math.cos(rho),sr=Math.sin(rho);   /* roulis : de « sur le dos » (π) à « roues vers le bas » (0) */
      pnPose(c,dt,F,tmp,qA,F.J.x+(F.C.x-F.J.x)*e,F.J.y+(F.C.y-F.J.y)*e,F.J.z+(F.C.z-F.J.z)*e,f,{x:sx/sl*cr+side.x*sr,y:sy/sl*cr+side.y*sr,z:sz/sl*cr+side.z*sr});
      c.boosting=u>.15;c.grounded=false;
    }
    return;
  }
  if(F.st===0){   /* contact : pinch au sol, la balle part vers le but */
    const v=pnVel(F.tB,45,{x:0,y:-1,z:0},1.08);
    b.frozen=0;b.vel.set(v.x,v.y,v.z);S.t1=t;S.pin={t:t,cs:45};F.st=1;burst(b.pos,0xffd166,22,18,.5);addShake(.3);
  }
  const dm=Math.max(0,1-dt*2.5),v=F.v;v.x*=dm;v.y=v.y*dm;v.z*=dm;
  pnPose(c,dt,F,tmp,qA,c.pos.x+v.x*dt,Math.max(CR+.3,c.pos.y+v.y*dt),Math.min(HZ-hz-2,c.pos.z+v.z*dt),{x:c.fwd.x,y:c.fwd.y,z:c.fwd.z},{x:0,y:1,z:0});
  c.boosting=false;
}

/* DÉMO SCRIPTÉE de l'air dribble : le coach (piloté image par image) pop la balle posée au sol, puis la retouche toutes les 0,5 s en restant derrière / sous elle,
   en la poussant vers le but adverse, et finit par un tir. La balle garde la vraie physique entre deux touches (seules les touches sont scriptées ; plan simulé : but). */
const AD_V=[[0,7,20],[0,7,22],[0,6,24]];   /* vitesses données à la balle aux touches n°2, 3 et 4 (la n°1 est le pop au sol, la n°5 est le tir) */
function adScript(c,dt,S,t){
  const b=ball,hz=c.hb?c.hb.hz:2.15,gap=BR+hz+.15,tmp={},qA={x:0,y:0,z:0,w:1};
  if(!S.ad)S.ad={ft:0,st:0,p0:{x:c.pos.x,y:c.pos.y,z:c.pos.z},f0:{x:c.fwd.x,y:c.fwd.y,z:c.fwd.z},v:{x:0,y:0,z:0},k:0,tt:0,TM:1.3,DT:.5};
  const F=S.ad;F.ft+=dt;
  const pose=(px,py,pz,fx,fy,fz)=>{
    const fl=Math.hypot(fx,fy,fz)||1,fwd={x:fx/fl,y:fy/fl,z:fz/fl},ud=fwd.y;
    const wx=-fwd.x*ud,wy=1-fwd.y*ud,wz=-fwd.z*ud,ul=Math.hypot(wx,wy,wz)||1,up={x:wx/ul,y:wy/ul,z:wz/ul};
    frBasis(up,fwd,tmp);frQuat(qA,tmp.r,up,tmp.b);
    const ox=c.pos.x,oy=c.pos.y,oz=c.pos.z;frApply(c,px,py,pz,qA);
    c.vel.set((px-ox)/dt,(py-oy)/dt,(pz-oz)/dt);F.v.x=c.vel.x;F.v.y=c.vel.y;F.v.z=c.vel.z;
  };
  const sy=.3,sz=.954;   /* direction « nez » pendant le dribble : un peu vers le haut, vers le but */
  if(F.st===0){
    /* élan au sol jusqu'à la balle posée */
    const u=Math.min(1,F.ft/F.TM),e=Math.pow(u,1.3),k2=frSS(.4,1,u);
    const tx=b.pos.x,ty=Math.max(CR+.02,b.pos.y-sy*gap),tz=b.pos.z-sz*gap;
    pose(F.p0.x+(tx-F.p0.x)*e,F.p0.y+(ty-F.p0.y)*e,F.p0.z+(tz-F.p0.z)*e,F.f0.x*(1-k2),F.f0.y+(sy-F.f0.y)*k2,F.f0.z+(sz-F.f0.z)*k2);
    c.boosting=u>.15;c.grounded=true;
    if(u>=1){   /* touche n°1 : le pop (au sol) */
      b.frozen=0;b.vel.set(0,14,12);F.st=1;F.k=1;F.tt=0;S.cT=t;burst(b.pos,0xffffff,14,14,.35);addShake(.25);
    }
    return;
  }
  if(F.st===1){
    /* dribble : la voiture reste sous / derrière la balle, se rapproche juste avant chaque touche */
    F.tt+=dt;
    const ph=Math.min(1,F.tt/F.DT),extra=3*(1-frSS(.55,1,ph))*frSS(0,.25,ph);
    pose(b.pos.x,Math.max(CR+.1,b.pos.y-sy*gap),b.pos.z-sz*gap-extra,0,sy,sz);
    c.boosting=ph>.3&&ph<.9;c.grounded=c.pos.y<CR+.3;
    if(F.tt>=F.DT){
      F.tt-=F.DT;F.k++;S.cT=t;S.air=(S.air||0)+1;
      if(F.k<=4){b.vel.set(AD_V[F.k-2][0],AD_V[F.k-2][1],AD_V[F.k-2][2]);burst(b.pos,0xbfe6ff,8,10,.25);}
      else{   /* touche n°5 : le tir dans le but */
        const dx=-b.pos.x,dy=2.8-b.pos.y,dz=HZ+6-b.pos.z,T=Math.max(.15,Math.hypot(dx,dy,dz)/50);
        b.vel.set(dx/T,dy/T+.5*G*T,dz/T);F.st=2;burst(b.pos,0xffffff,18,20,.35);addShake(.3);
      }
    }
    return;
  }
  /* après le tir : la voiture finit sa course sans entrer dans la cage */
  const dm=Math.max(0,1-dt*1.5),v=F.v;v.x*=dm;v.y=v.y*dm-G*.25*dt;v.z*=dm;
  pose(c.pos.x+v.x*dt,Math.max(CR+.3,c.pos.y+v.y*dt),Math.min(HZ-hz-2,c.pos.z+v.z*dt),c.fwd.x,c.fwd.y,c.fwd.z);
  c.boosting=false;
}

/* DÉMO SCRIPTÉE du double tap : le coach est piloté image par image (sans physique de voiture) pour réussir à tous les coups :
   aérien vers la balle figée → 1re touche (la balle part sur le tableau arrière, au-dessus de la cage) → il la suit → au rebond, 2e touche en l'air qui l'envoie dans le but.
   La balle garde la vraie physique : seuls les deux « coups » (vitesse donnée à la balle) sont scriptés, le rebond est le vrai. */
const DT_PITCH=.66;   /* inclinaison du nez au moment de la 1re touche (rad) */
function dtShot(b,tx,ty,tz,sp){ /* donne à la balle la vitesse qui l'amène en (tx,ty,tz) en tenant compte de la gravité */
  const dx=tx-b.pos.x,dy=ty-b.pos.y,dz=tz-b.pos.z,T=Math.max(.15,Math.hypot(dx,dy,dz)/sp);
  b.vel.set(dx/T,dy/T+.5*G*T,dz/T);
  return {x:dx,y:dy,z:dz};
}
function dtScript(c,dt,S,t){
  const b=ball,hz=c.hb?c.hb.hz:2.15,gap=BR+hz+.15,tmp={},qA={x:0,y:0,z:0,w:1};
  if(!S.dt){
    S.dt={ft:0,st:0,p0:{x:c.pos.x,y:c.pos.y,z:c.pos.z},bp:{x:b.pos.x,y:b.pos.y,z:b.pos.z},TA:1.75,TB:.55};
    const f=S.dt,fw={x:0,y:Math.sin(DT_PITCH),z:Math.cos(DT_PITCH)};
    f.cp={x:f.bp.x-fw.x*gap,y:f.bp.y-fw.y*gap,z:f.bp.z-fw.z*gap};   /* position de la voiture à la 1re touche : nez sur la balle, par en dessous */
    f.v={x:0,y:0,z:0};f.sd={x:0,y:-.3,z:1};
  }
  const F=S.dt;F.ft+=dt;
  const pose=(px,py,pz,fx,fy,fz,ux,uy,uz)=>{
    const fl=Math.hypot(fx,fy,fz)||1,fwd={x:fx/fl,y:fy/fl,z:fz/fl},ud=ux*fwd.x+uy*fwd.y+uz*fwd.z;
    let wx=ux-fwd.x*ud,wy=uy-fwd.y*ud,wz=uz-fwd.z*ud;const ul=Math.hypot(wx,wy,wz)||1;const up={x:wx/ul,y:wy/ul,z:wz/ul};   /* up rendu perpendiculaire à l'avant : la voiture garde bien son inclinaison */
    frBasis(up,fwd,tmp);frQuat(qA,tmp.r,up,tmp.b);
    const ox=c.pos.x,oy=c.pos.y,oz=c.pos.z;frApply(c,px,py,pz,qA);
    c.vel.set((px-ox)/dt,(py-oy)/dt,(pz-oz)/dt);F.v.x=c.vel.x;F.v.y=c.vel.y;F.v.z=c.vel.z;
  };
  if(F.st===0){
    /* 1) élan au sol, puis aérien : la voiture monte vers la balle en se cabrant (boost) */
    const u=Math.min(1,F.ft/F.TA),e=Math.pow(u,1.35),ey=Math.pow(u,2.2),p=DT_PITCH*frSS(.12,.7,u);
    pose(F.p0.x+(F.cp.x-F.p0.x)*e,F.p0.y+(F.cp.y-F.p0.y)*ey,F.p0.z+(F.cp.z-F.p0.z)*e,0,Math.sin(p),Math.cos(p),0,Math.cos(p),-Math.sin(p));
    c.boosting=u>.05;c.grounded=u<.1;
    if(u>=1){ /* 1re touche : la balle file sur le tableau arrière, au-dessus de la cage (tz = un peu « dans » le mur pour être sûr du contact) */
      b.frozen=0;dtShot(b,Math.max(-GW*.6,Math.min(GW*.6,b.pos.x)),13.2,HZ-BR+.4,58);
      S.t1=t;F.st=1;burst(b.pos,0xffd166,16,14,.4);addShake(.3);
    }
    return;
  }
  if(F.st===1){
    /* 2) la voiture continue sur sa lancée, derrière la balle, sans traverser le mur du fond */
    const dm=Math.max(0,1-dt*.8),v=F.v;v.x*=dm;v.y=v.y*dm;v.z*=dm;
    pose(c.pos.x+v.x*dt,Math.min(H-3,c.pos.y+v.y*dt),Math.min(HZ-hz-2,c.pos.z+v.z*dt),c.fwd.x,c.fwd.y,c.fwd.z,c.up.x,c.up.y,c.up.z);
    c.boosting=true;c.grounded=false;
    if(S.bb!==undefined){   /* rebond détecté : on se cale sur la balle pour la 2e touche */
      F.st=2;F.tb=F.ft;F.o0={x:c.pos.x-b.pos.x,y:c.pos.y-b.pos.y,z:c.pos.z-b.pos.z};F.f0={x:c.fwd.x,y:c.fwd.y,z:c.fwd.z};
      const gx=Math.max(-GW*.5,Math.min(GW*.5,-b.pos.x*.5)),dx=gx-b.pos.x,dy=3.4-b.pos.y,dz=HZ+6-b.pos.z,dl=Math.hypot(dx,dy,dz)||1;
      F.sd={x:dx/dl,y:dy/dl,z:dz/dl};F.gx=gx;
    }
    return;
  }
  if(F.st===2){
    /* 3) elle revient sur la balle qui rebondit, nez vers le but, et la touche en l'air */
    const u=Math.min(1,(F.ft-F.tb)/F.TB),e=frSS(0,1,u),sd=F.sd;
    const fx=F.f0.x+(sd.x-F.f0.x)*e,fy=F.f0.y+(sd.y-F.f0.y)*e,fz=F.f0.z+(sd.z-F.f0.z)*e;
    pose(b.pos.x+F.o0.x*(1-e)-sd.x*gap*e,b.pos.y+F.o0.y*(1-e)-sd.y*gap*e,b.pos.z+F.o0.z*(1-e)-sd.z*gap*e,fx,fy,fz,0,1,0);
    c.boosting=u<.7;c.grounded=false;
    if(u>=1){ /* 2e touche : la balle part dans le but (on vise un peu à l'intérieur de la cage) */
      dtShot(b,F.gx,3.4,HZ+6,52);S.t2=t;F.st=3;burst(b.pos,0xffffff,18,20,.35);addShake(.3);
    }
    return;
  }
  /* 4) après la frappe : la voiture finit sa course (sans entrer dans la cage) */
  const dm=Math.max(0,1-dt*1.5),v=F.v;v.x*=dm;v.y=v.y*dm-G*.25*dt;v.z*=dm;
  pose(c.pos.x+v.x*dt,Math.max(CR+.3,c.pos.y+v.y*dt),Math.min(HZ-hz-1,c.pos.z+v.z*dt),c.fwd.x,c.fwd.y,c.fwd.z,c.up.x,c.up.y,c.up.z);
  c.boosting=false;
}

/* ---------- moteur du pack mécaniques ---------- */
function mvProg(id){SV.mv=SV.mv||{};return SV.mv[id]||(SV.mv[id]={ok:0,att:0});}
function mvInit(){
  const co=newCar(1,0,false,'COACH',1);co.coach=true;co.ghost=true;co.mesh.visible=false;cars.push(co);MV.coach=co;
  MV.i=Math.min(MOVES.length-1,Math.max(0,cfg.mv||0));MV.demo=cfg.mvDemo!==0;
  MV.att=0;MV.ok=0;
  $('mv').classList.remove('hide');
  mvLoad(MV.i);
}
function mvLoad(i){
  MV.i=(i+MOVES.length)%MOVES.length;MV.att=0;MV.ok=0;
  mvRender();
  if(MV.demo)mvBeginDemo();else mvBeginTry(true);
}
function mvCamSnap(c){
  const f=c.fwd;camHead.set(f.x,0,f.z);if(camHead.lengthSq()<.01)camHead.set(0,0,1);camHead.normalize();
  camPos.copy(c.pos).addScaledVector(camHead,-12);camPos.y=Math.max(5,c.pos.y+4);clampCam(camPos);
}
function mvBeginDemo(){
  const m=MOVES[MV.i],p=player,co=MV.coach;
  if(m.noDemo){mvBeginTry(true);return;}   /* pas de démo du coach : c'est le joueur qui fait tout */
  p.ghost=true;p.mesh.visible=false;
  m.setup(co,true);
  MV.car=co;MV.state='demo';MV.t=0;MV.S={};MV.demoOK=null;co.prevJ=false;
  mvCamSnap(co);clearMsg();
}
function mvBeginTry(first){
  const m=MOVES[MV.i],p=player,co=MV.coach;
  co.ghost=true;co.mesh.visible=false;co.in.jumpEdge=false;
  m.setup(p,false);
  MV.car=p;MV.state='try';MV.t=0;MV.S={};
  prevJump=!!(keys.Space||btn.jump);
  mvCamSnap(p);
  if(first)showMsg('À TOI !',m.goal,'#7dffb0',2.2);
}
function mvSkip(){if(MV.state==='demo'||MV.state==='demoEnd')mvBeginTry(true);}
function mvReplay(){mvBeginDemo();}
function coachStep(dt){
  const m=MOVES[MV.i],c=MV.coach,I=c.in;
  I.x=I.y=I.thr=I.roll=0;I.boost=I.jump=I.drift=I.rollMode=false;
  if(c.dead||c.ghost)return;
  m.demo(c,MV.t,I,MV.S);
  if(I.jump&&!c.prevJ)I.jumpEdge=true;c.prevJ=I.jump;
}
function mvResult(ok,msg){
  const m=MOVES[MV.i];
  if(MV.state==='demo'){
    if(ok){if(!MV.S.won){MV.S.won=true;MV.S.wonT=MV.t;MV.demoOK=true;popup('Démo réussie',0,'#7dffb0');}}
    else{MV.demoOK=false;MV.state='demoEnd';MV.wait=.9;}
    return;
  }
  if(MV.state!=='try')return;
  MV.state='wait';MV.wait=1.6;MV.att++;if(ok)MV.ok++;
  const pg=mvProg(m.id);pg.att++;if(ok)pg.ok++;saveSV();
  showMsg(ok?'RÉUSSI !':'RATÉ',(msg||MV.S.msg||'')+(msg||MV.S.msg?'  •  ':'')+MV.ok+' / '+MV.att,ok?'#7dffb0':'#ff8a8a',1.4);
  if(ok){SFX.beep(true);addShake(.25);}
}
function mvTick(dt){
  if(!MODE.moves||phase!=='play'||!MV.car)return;
  if(MV.state!=='demo'&&MV.state!=='try')return;
  const m=MOVES[MV.i],c=MV.car,S=MV.S;
  MV.t+=dt;
  if(MV.state==='demo'&&S.won){if(MV.t>S.wonT+(m.after||1.2)){MV.state='demoEnd';MV.wait=.6;}return;}
  let r;
  if(m.tick)r=m.tick(c,dt,S,MV.t);
  if(r===undefined&&MV.t>m.lim){r=false;S.msg=S.msg||'Temps écoulé';}
  if(r!==undefined)mvResult(r);
}
/* w = les 4 roues sur la balle ; rs = un flip dépensé a vraiment été rendu (flip reset valide) */
function mvTouch(c,nx,ny,nz,hit,w,rs){
  if(c!==MV.car||(MV.state!=='demo'&&MV.state!=='try'))return;
  const m=MOVES[MV.i];
  if(m.touch&&!(MV.state==='demo'&&MV.S.won)){const r=m.touch(c,{nx:nx,ny:ny,nz:nz,hit:hit,air:!c.grounded,bs:ball.vel.length(),w:!!w,rs:!!rs},MV.S,MV.t);if(r!==undefined)mvResult(r);}
}
function mvLand(c,fl){
  if(c!==MV.car||(MV.state!=='demo'&&MV.state!=='try'))return;
  const m=MOVES[MV.i];
  if(m.land&&!(MV.state==='demo'&&MV.S.won)){const r=m.land(c,{fl:fl,age:c.dodgeAge,sp:c.vel.length()},MV.S,MV.t);if(r!==undefined)mvResult(r);}
}
function mvUpdate(dt){
  if(!MODE.moves||phase!=='play')return;
  if(MV.state==='demoEnd'){MV.wait-=dt;if(MV.wait<=0)mvBeginTry(true);}
  else if(MV.state==='wait'){MV.wait-=dt;if(MV.wait<=0)mvBeginTry(false);}
  const st=MV.state==='demo'||MV.state==='demoEnd'?'🎬 DÉMO DU COACH':'🎯 À TOI DE JOUER';
  const h=st+'|'+MV.ok+'|'+MV.att;
  if(h!==MV._hud){MV._hud=h;$('mvS').textContent=st;$('mvC').textContent='Réussites : '+MV.ok+' / '+MV.att;$('mvS').className=MV.state==='demo'||MV.state==='demoEnd'?'demo':'try';}
}
function mvRender(){
  const m=MOVES[MV.i];
  $('mvT').innerHTML=m.ic+' '+m.n+' <small>'+m.lvl+' • '+(MV.i+1)+'/'+MOVES.length+'</small>';
  $('mvD').textContent=m.def;
  $('mvL').innerHTML=m.steps.map(s=>'<li>'+s+'</li>').join('');
  $('mvG').textContent='Objectif : '+m.goal;
  MV._hud='';
}
function mvGoal(team){
  if(MV.state!=='demo'&&MV.state!=='try')return;
  const m=MOVES[MV.i];
  if(m.goal_&&!(MV.state==='demo'&&MV.S.won)){const r=m.goal_(MV.car,team,MV.S);if(r!==undefined)mvResult(r);}
}

/* ---------- tournoi ---------- */
const TOUR_NAMES=['Les Fusées','Turbo Club','Néon FC','Rocket Pack','Drift Kings','Les Comètes','Apex United','Blaze Squad','Vortex Team','Pixel Rush','Zéro Gravité','Flux Crew'];
function tourNew(){
  const rg=mulberry((Date.now()/1000)|0),pool=TOUR_NAMES.slice().sort(()=>rg()-.5);
  const teams=[(SV.name||'Joueur')+' & co'].concat(pool.slice(0,7));
  SV.tour={round:0,alive:true,teams:teams,res:[[],[],[]],champ:false};saveSV();
}
const TOUR_ROUNDS=['Quarts de finale','Demi-finale','Finale'];
const TOUR_PRIZE=[300,700,2000];
/* =====================  PROGRESSION / RÉCOMPENSES  ===================== */
const RANKS=['Bronze','Argent','Or','Platine','Diamant','Champion','Grand Champion'];
const RANKC=['#b8793a','#c0c8d4','#ffcc33','#5fe0d0','#6aa8ff','#c27bff','#ff6a6a'];
function rankOf(mmr){
  const i=clamp(Math.floor(mmr/60),0,21);
  if(i>=21)return {n:'Supersonic Legend',c:'#ffffff',i:21};
  return {n:RANKS[Math.floor(i/3)]+' '+(i%3+1),c:RANKC[Math.floor(i/3)],i:i};
}
/* Compétitif : on démarre « Non classé » ; le rang n'apparaît qu'après 10 matchs de placement par format */
const PLACE_N=10;
function isRk(k){return k==='ranked'||String(k).indexOf('rk_')===0;}
function rkKey(k,size){const b=String(k).replace(/^rk_/,'');return (b==='ranked'?'':b+'_')+size;}
function rankInfo(size){
  const key=''+size,pl=(SV.plc&&SV.plc[key])||0;
  if(pl<PLACE_N)return {n:'Non classé',c:'#9aa4b8',i:-1,un:true,left:PLACE_N-pl};
  return rankOf(mmrOf(key));
}
const CHPOOL=[
  {id:'goals',n:'Marque {n} buts',k:'goals',v:[3,5,8],cr:[200,350,550]},
  {id:'wins',n:'Gagne {n} match(s)',k:'wins',v:[1,2,3],cr:[250,450,700]},
  {id:'demos',n:'Fais {n} démolitions',k:'demos',v:[2,4,7],cr:[200,350,550]},
  {id:'saves',n:'Fais {n} arrêts',k:'saves',v:[2,4,6],cr:[200,350,500]},
  {id:'matches',n:'Joue {n} matchs',k:'matches',v:[2,3,5],cr:[150,300,450]},
  {id:'assists',n:'Fais {n} passes décisives',k:'assists',v:[1,3,5],cr:[200,400,600]},
  {id:'rumble',n:'Joue {n} match(s) Rumble',k:'m_rumble',v:[1,2,3],cr:[250,400,600]},
  {id:'hoops',n:'Joue {n} match(s) Hoops',k:'m_hoops',v:[1,2,3],cr:[250,400,600]},
  {id:'drop',n:'Joue {n} match(s) Dropshot',k:'m_drop',v:[1,2,3],cr:[250,400,600]},
  {id:'snow',n:'Joue {n} match(s) Snow Day',k:'m_snow',v:[1,2,3],cr:[250,400,600]},
  {id:'heat',n:'Joue {n} match(s) Heatseeker',k:'m_heat',v:[1,2,3],cr:[250,400,600]}
];
function chToday(){
  const rg=mulberry(dayNum()*31+7),pool=CHPOOL.slice().sort(()=>rg()-.5).slice(0,4);
  return pool.map((c,i)=>{const t=i===3?2:(rg()*2)|0;return {id:c.id+'_'+t,k:c.k,v:c.v[t],cr:c.cr[t],xp:c.cr[t]*.6|0,n:c.n.replace('{n}',c.v[t])};});
}
function chSync(){if(SV.ch.day!==todayKey()){SV.ch={day:todayKey(),prog:{},claimed:{}};saveSV();}}
function chAdd(k,n){chSync();chToday().forEach(c=>{if(c.k===k)SV.ch.prog[c.id]=(SV.ch.prog[c.id]||0)+n;});ch2Add(k,n);ndAdd(k,n);}
/* passe de saison : 40 paliers */
const PASS_XP=500,PASS=[];
{
  const rg=mulberry(777),cats=['wheel','boost','topper','ant','expl','trail','decal','eng','ban'];
  const pick=(cat,f)=>{let c=LST[cat].filter(i=>i.price>0&&f(i));if(!c.length)c=LST[cat].filter(i=>i.price>0);return c[(rg()*c.length)|0].id;};
  for(let t=1;t<=40;t++){
    if(t%10===0)PASS.push({t:t,type:'item',val:pick('car',i=>i.r>=2&&i.r<=4)});
    else if(t%5===0)PASS.push({t:t,type:'item',val:pick(cats[(rg()*cats.length)|0],i=>i.r>=1)});
    else if(t%3===0&&t<38)PASS.push({t:t,type:'item',val:pick(cats[(rg()*cats.length)|0],i=>i.r<=1)});
    else PASS.push({t:t,type:'cr',val:100+t*12});
  }
}
function passTier(){return Math.min(40,Math.floor(SV.xp/PASS_XP));}
function passSync(){
  const got=[];
  while(SV.pass.claimed<passTier()){
    const r=PASS[SV.pass.claimed];SV.pass.claimed++;
    if(r.type==='cr')SV.credits+=r.val;else SV.owned[r.val]=1;
    got.push(r);
  }
  return got;
}
/* daily / boutique / caisses */
function claimDaily(){
  const today=todayKey();if(SV.daily.last===today)return 0;
  const y=new Date(Date.now()-864e5),yk=y.getFullYear()+'-'+(y.getMonth()+1)+'-'+y.getDate();
  SV.daily.streak=SV.daily.last===yk?Math.min(7,SV.daily.streak+1):1;SV.daily.last=today;
  const r=150+SV.daily.streak*50;SV.credits+=r;saveSV();return r;
}
function shopPick(seed,n,minR,maxR,exclude){
  const rg=mulberry(seed),all=[];
  for(const c of CATS)if(c.k!=='fin')LST[c.k].forEach(it=>{if(it.price>0&&it.r>=minR&&it.r<=maxR&&!(exclude&&exclude[it.id]))all.push(it);});
  const out=[];while(out.length<n&&all.length){const j=(rg()*all.length)|0;out.push(all.splice(j,1)[0]);}
  return out;
}
function shopDaily(){return shopPick(dayNum()*13+5,8,0,3);}
function shopFeatured(){return shopPick(dayNum()*17+9,4,2,5);}
const CRATES=[
  {n:'Caisse Argent',price:500,w:[60,30,8,2,0,0],c:'#c0c8d4'},
  {n:'Caisse Or',price:1500,w:[20,40,28,10,2,0],c:'#ffcc33'},
  {n:'Caisse Marché noir',price:4000,w:[0,15,35,30,15,5],c:'#ff4fd8'}
];
function openCrate(i){
  const cr=CRATES[i];if(SV.credits<cr.price)return null;
  SV.credits-=cr.price;
  let tot=cr.w.reduce((a,b)=>a+b,0),x=rnd()*tot,r=0;
  for(let k=0;k<6;k++){x-=cr.w[k];if(x<=0){r=k;break;}}
  let pool=[];for(const c of CATS)if(c.k!=='fin')LST[c.k].forEach(it=>{if(it.price>0&&it.r===r)pool.push(it);});
  if(!pool.length)pool=LST.wheel.filter(it=>it.price>0);
  const it=pool[(rnd()*pool.length)|0];let dup=false,refund=0;
  if(owns(it.id)){dup=true;refund=Math.round(it.price*.35);SV.credits+=refund;}else SV.owned[it.id]=1;
  SV.crates=(SV.crates||0)+1;saveSV();return {it:it,dup:dup,refund:refund};
}
function buyItem(id){
  const it=IT[id];if(!it||owns(id))return false;
  if(SV.credits<it.price)return false;
  SV.credits-=it.price;SV.owned[id]=1;saveSV();return true;
}
const BUNDLES=[];
{
  const specs=[[10,3,2,3,'topper',3],[27,4,5,6,'ant',2],[41,8,9,7,'topper',9],[58,11,12,10,'ant',6],[73,14,15,13,'topper',12],[90,17,18,16,'ant',10]];
  specs.forEach((s,k)=>{
    const ids=['car:'+s[0],'wheel:'+s[1],'decal:'+s[2],'boost:'+s[3],s[4]+':'+s[5],'trail:'+(k+2),'expl:'+(k+2)];
    BUNDLES.push({n:'Lot '+IT['car:'+s[0]].n,ids:ids.filter(i=>IT[i])});
  });
}
function bundlePrice(b){let s=0;b.ids.forEach(i=>{if(!owns(i))s+=IT[i].price;});return Math.round(s*.72);}
function buyBundle(b){
  const p=bundlePrice(b);if(p<=0||SV.credits<p)return false;
  SV.credits-=p;b.ids.forEach(i=>{SV.owned[i]=1;});saveSV();return true;
}
/* fin de match : gains, rang, défis */
function matchRewards(win){
  const st=player?player.stats:{goals:0,assists:0,saves:0,shots:0,demos:0,score:0};
  const mult=MODE.ranked?1.4:1;
  let cr=Math.round((60+st.goals*30+st.assists*15+st.saves*20+st.demos*10+(win?180:0))*mult),xp=Math.round((90+st.score*.6+(win?90:0))*mult);
  const res={cr:cr,xp:xp,item:null,mmr:0,rank:null,levelUp:false,tier:[],ch:[]};
  const lv0=lvlOf(SV.xp);
  SV.credits+=cr;SV.xp+=xp;
  const S=SV.stats;S.matches++;if(win)S.wins++;S.goals+=st.goals;S.assists+=st.assists;S.saves+=st.saves;S.shots+=st.shots;S.demos+=st.demos;
  const nd0=ndSync(),ndWas=nd0.done;
  chSync();chAdd('matches',1);if(win)chAdd('wins',1);chAdd('goals',st.goals);chAdd('assists',st.assists);chAdd('saves',st.saves);chAdd('demos',st.demos);
  const mk={rumble:'m_rumble',hoops:'m_hoops',drop:'m_drop',snow:'m_snow',heat:'m_heat'}[MODE.k];if(mk)chAdd(mk,1);
  res.ndDone=!ndWas&&nd0.done;
  if(MODE.ranked){
    const key=rkKey(cfg.mode,cfg.size),old=mmrOf(key);
    SV.plc=SV.plc||{};const pl=SV.plc[key]||0,place=pl<PLACE_N;
    const d=place?(win?(40+((rnd()*21)|0)):-(20+((rnd()*11)|0))):(win?(9+((rnd()*5)|0)):-(8+((rnd()*4)|0)));
    SV.mmr[key]=Math.max(0,old+d);SV.plc[key]=pl+1;res.mmr=d;res.rank=rankInfo(key);
  }
  if(rnd()<(win?.35:.18)){
    const it=shopPick((Date.now()/1000)|0,1,0,win?3:2,SV.owned)[0];
    if(it&&!owns(it.id)){SV.owned[it.id]=1;res.item=it;}
  }
  res.tier=passSync();res.levelUp=lvlOf(SV.xp)>lv0;
  saveSV();return res;
}

/* =====================================================================
   LOBBY 3D, APERÇUS DU CASIER ET MINIATURES
   ===================================================================== */
const lobbyScene=new T.Scene();
lobbyScene.background=new T.Color(0x060914);
lobbyScene.fog=new T.Fog(0x060914,55,170);
lobbyScene.environment=scene.environment;
const lobby={car:null,cobj:null,mates:[],matesN:-1,yaw:.5,yawT:.5,ctx:'lobby',cat:'car',L:null,boostOn:false,trailOn:false,t:0,fov:36,
  cam:{x:3.6,y:1.7,z:0,d:13,h:2.8},tgt:{x:3.6,y:1.7,z:0,d:13,h:2.8},dust:null,drag:false,idle:0,arch:null};
const lobbyPoints=new T.Points(pGeo,points.material);lobbyPoints.frustumCulled=false;lobbyScene.add(lobbyPoints);

function buildLobbyScene(){
  const sc=lobbyScene;
  sc.add(new T.HemisphereLight(0xbcd2ff,0x141a2c,.95));
  const key=new T.DirectionalLight(0xffffff,.95);key.position.set(6,12,9);sc.add(key);
  const p1=new T.PointLight(0x2f8bff,1.5,70);p1.position.set(-12,6,-8);sc.add(p1);
  const p2=new T.PointLight(0xff8a2a,1.5,70);p2.position.set(12,6,-8);sc.add(p2);
  const spot=new T.SpotLight(0xffffff,1.7,50,.55,.5);spot.position.set(0,18,3);spot.target.position.set(0,0,0);sc.add(spot);sc.add(spot.target);
  // sol brillant
  const fl=new T.Mesh(new T.CircleGeometry(90,56),new T.MeshStandardMaterial({color:0x0a1022,metalness:.85,roughness:.3}));fl.rotation.x=-Math.PI/2;sc.add(fl);
  for(const r of [14,22,34]){const t=new T.Mesh(new T.TorusGeometry(r,.1,6,80),new T.MeshBasicMaterial({color:0x2f8bff,transparent:true,opacity:.35}));t.rotation.x=Math.PI/2;t.position.y=.03;sc.add(t);}
  // plateforme
  const plat=new T.Mesh(new T.CylinderGeometry(6.2,6.7,.5,56),new T.MeshStandardMaterial({color:0x141c34,metalness:.7,roughness:.35}));plat.position.y=.0;sc.add(plat);
  const ringA=new T.Mesh(new T.TorusGeometry(6.1,.14,8,72),new T.MeshBasicMaterial({color:0x2f8bff}));ringA.rotation.x=Math.PI/2;ringA.position.y=.26;sc.add(ringA);
  const ringB=new T.Mesh(new T.TorusGeometry(4.6,.07,8,72),new T.MeshBasicMaterial({color:0xff8a2a}));ringB.rotation.x=Math.PI/2;ringB.position.y=.26;sc.add(ringB);
  const glow=new T.Mesh(new T.PlaneGeometry(22,22),new T.MeshBasicMaterial({map:TX.glow,color:0x2f8bff,transparent:true,opacity:.55,blending:T.AdditiveBlending,depthWrite:false}));glow.rotation.x=-Math.PI/2;glow.position.y=.02;sc.add(glow);
  // mur hexagonal circulaire
  const wt=TX.wall.clone();wt.needsUpdate=true;wt.wrapS=wt.wrapT=T.RepeatWrapping;wt.repeat.set(12,1.2);
  const wall=new T.Mesh(new T.CylinderGeometry(60,60,34,56,1,true),new T.MeshBasicMaterial({map:wt,color:0x3a6ad0,transparent:true,opacity:.9,side:T.BackSide,depthWrite:false}));wall.position.y=17;sc.add(wall);
  for(let i=0;i<28;i++){
    const a=i/28*Math.PI*2,s=new T.Mesh(new T.BoxGeometry(.6,30,.6),new T.MeshBasicMaterial({color:i%2?0x2f8bff:0xff8a2a}));
    s.position.set(Math.cos(a)*58,15,Math.sin(a)*58);sc.add(s);
  }
  // cage (aperçu des explosions de but)
  const arch=new T.Group(),fm=new T.MeshBasicMaterial({color:0xffffff});
  const fb=(x,y,w,h)=>{const m=new T.Mesh(new T.BoxGeometry(w,h,.5),fm);m.position.set(x,y,0);arch.add(m);};
  fb(0,9,20.6,.6);fb(-10,4.5,.6,9);fb(10,4.5,.6,9);
  const net=new T.Mesh(new T.PlaneGeometry(20,9,10,5),new T.MeshBasicMaterial({color:0x7fb2ff,wireframe:true,transparent:true,opacity:.5}));net.position.set(0,4.5,-.2);arch.add(net);
  arch.position.set(0,0,-24);sc.add(arch);lobby.arch=arch;
  // poussière
  const N=160,pa=new Float32Array(N*3);for(let i=0;i<N;i++){pa[i*3]=(rnd()-.5)*60;pa[i*3+1]=rnd()*20;pa[i*3+2]=(rnd()-.5)*60;}
  const dg=new T.BufferGeometry();dg.setAttribute('position',new T.BufferAttribute(pa,3));
  lobby.dust=new T.Points(dg,new T.PointsMaterial({size:.45,map:TX.glow,color:0x9fc4ff,transparent:true,opacity:.7,depthWrite:false,blending:T.AdditiveBlending,fog:false}));
  lobby.dust.frustumCulled=false;sc.add(lobby.dust);
}
buildLobbyScene();
lobby.hall=lobbyScene.children.filter(c=>c.isMesh);
function buildLobbyStadium(){
  const g=new T.Group();
  const cv=(w,h,fn)=>{const c=document.createElement('canvas');c.width=w;c.height=h;fn(c.getContext('2d'),w,h);return new T.CanvasTexture(c);};
  const grassT=cv(1024,1024,(x,w,h)=>{for(let i=0;i<16;i++){x.fillStyle=i%2?'#1c5a22':'#26702a';x.fillRect(i*w/16,0,w/16,h);}
    x.fillStyle='rgba(45,120,255,.6)';x.fillRect(0,300,330,100);x.fillRect(0,560,560,60);
    x.strokeStyle='rgba(255,255,255,.9)';x.lineWidth=8;x.beginPath();x.ellipse(780,300,380,210,0,0,Math.PI*2);x.stroke();x.beginPath();x.moveTo(0,720);x.lineTo(1024,650);x.stroke();});
  grassT.wrapS=grassT.wrapT=T.RepeatWrapping;grassT.repeat.set(3,3);
  const grass=new T.Mesh(new T.PlaneGeometry(240,240),new T.MeshStandardMaterial({map:grassT,color:0x6c8a6c,roughness:1,metalness:0}));grass.rotation.x=-Math.PI/2;g.add(grass);
  const adT=cv(1024,64,(x,w,h)=>{x.fillStyle='#071a45';x.fillRect(0,0,w,h);x.font='italic 800 34px sans-serif';x.fillStyle='#fff';for(let i=0;i<3;i++)x.fillText('ROCKET ARENA',24+i*340,44);x.fillStyle='#ff8a2a';x.fillRect(0,0,w,5);});
  adT.wrapS=T.RepeatWrapping;adT.repeat.set(5,1);
  const board=new T.Mesh(new T.BoxGeometry(200,2.4,.6),new T.MeshBasicMaterial({map:adT}));board.position.set(0,1.2,-30);g.add(board);
  const crowdT=cv(1024,512,(x,w,h)=>{x.fillStyle='#0a1024';x.fillRect(0,0,w,h);const P=['#e9b24d','#d4552b','#3a6ad0','#cfd8ff','#8a93a6','#ff8a2a','#2f8bff'];
    for(let i=0;i<9000;i++){x.fillStyle=P[(Math.random()*P.length)|0];x.globalAlpha=.35+Math.random()*.5;x.fillRect(Math.random()*w,Math.random()*h,3,4);}
    x.globalAlpha=1;for(let r=0;r<16;r++){x.fillStyle='rgba(0,0,10,.55)';x.fillRect(0,r*32+26,w,6);}});
  const mk=(w,x0,z0,ry)=>{const m=new T.Mesh(new T.PlaneGeometry(w,46),new T.MeshBasicMaterial({map:crowdT,color:0x8592b0}));m.rotation.order='YXZ';m.rotation.set(-.82,ry,0);m.position.set(x0,18.2,z0);g.add(m);};
  mk(230,0,-48,0);
  for(const x of [-70,-26,26,70]){const sp=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xffffff,blending:T.AdditiveBlending,transparent:true,depthWrite:false,fog:false}));sp.position.set(x,44,-60);sp.scale.set(46,46,1);g.add(sp);}
  g.visible=false;lobbyScene.add(g);lobby.stad=g;
}
buildLobbyStadium();

/* ---------- voiture du lobby ---------- */
function lobbyRefresh(L){
  lobby.L=L||lobby.L||SV.load;
  const Ld=lobby.L;
  if(lobby.car)lobbyScene.remove(lobby.car);
  lobby.car=makeCarMesh(Ld,SV.menu.team||0,true);
  lobby.car.position.set(0,1.15,0);lobby.car.rotation.y=lobby.yaw;lobbyScene.add(lobby.car);
  lobby.cobj={pos:lobby.car.position,fwd:V3(0,0,-1),right:V3(1,0,0),up:V3(0,1,0),vel:V3(0,0,-20),mesh:lobby.car,tfx:trailFx(Ld.trail),bfx:boostFx(Ld.boost),fakeV:9};
  lobbyFocus(lobby.cat);
}
function lobbyMates(names){
  const key=names.join('|');
  if(lobby.matesKey===key&&lobby.mates.length===names.length)return;
  lobby.mates.forEach(m=>lobbyScene.remove(m));lobby.mates=[];lobby.matesKey=key;
  const pos=[[-7.6,-3],[7.6,-3],[0,-10]];
  names.slice(0,3).forEach((nm,i)=>{
    const g=new T.Group();
    const pl=new T.Mesh(new T.CylinderGeometry(3.6,3.9,.4,36),new T.MeshStandardMaterial({color:0x141c34,metalness:.7,roughness:.35}));g.add(pl);
    const rg=new T.Mesh(new T.TorusGeometry(3.5,.09,8,48),new T.MeshBasicMaterial({color:i%2?0xff8a2a:0x2f8bff}));rg.rotation.x=Math.PI/2;rg.position.y=.21;g.add(rg);
    let hs=17;for(let c=0;c<nm.length;c++)hs=(hs*31+nm.charCodeAt(c))|0;
    const L=randLoad(mulberry(hs));
    const car=makeCarMesh(L,SV.menu.team||0,false);car.position.y=1.0;car.scale.setScalar(.85);car.userData.mateYaw=.5+i;g.add(car);
    const lb=makeLabel(nm,TCSS[SV.menu.team||0]);lb.position.set(0,3.6,0);g.add(lb);
    g.position.set(pos[i][0],0,pos[i][1]);g.userData.car=car;g.visible=false;lobbyScene.add(g);lobby.mates.push(g);
  });
}
/* ---------- mise au point de la caméra selon l'onglet ---------- */
function rotXZ(th,x,z){return [x*Math.cos(th)+z*Math.sin(th),-x*Math.sin(th)+z*Math.cos(th)];}
function lobbyFocus(cat){
  lobby.cat=cat;
  const P=carParams((lobby.L||SV.load).car),t=lobby.tgt,por=camera.aspect<1;
  let d=13,tx=0,ty=1.7,tz=0,h=2.8,yaw=null,boost=false,trail=false;
  switch(lobby.ctx==='lobby'?'lobby':cat){
    case 'wheel':{yaw=-Math.PI/2+.5;const r=rotXZ(yaw,P.w/2,-P.wb);d=6.2;tx=r[0];tz=r[1];ty=.6;h=1.3;break;}
    case 'topper':{yaw=-.7;const r=rotXZ(yaw,0,-(P.cab[2][0]+P.cab[3][0])/2);d=6.8;tx=r[0];tz=r[1];ty=3.0;h=1.4;break;}
    case 'ant':{yaw=.5;const r=rotXZ(yaw,P.w*.28,-P.body[5][0]*.9);d=6.8;tx=r[0];tz=r[1];ty=2.5;h=1.6;break;}
    case 'boost':{yaw=.15;d=10;ty=1.5;h=2.2;boost=true;break;}
    case 'trail':{yaw=-.5;d=11;ty=1.3;h=2.2;trail=true;break;}
    case 'decal':{yaw=-Math.PI/2+.35;d=10.5;ty=1.4;h=2.2;break;}
    case 'expl':{yaw=.35;d=15;ty=3.2;h=3;break;}
    case 'lobby':yaw=-.85;d=10+((PARTY.length&&UI.home)?6:0);ty=1.35;h=1.4;break;
    default:{yaw=-.7;d=12.5;ty=1.6;h=2.6;}
  }
  t.d=d;t.y=ty;t.h=h;
  const sx=por?0:(UI.home?-d*.05:d*(lobby.ctx==='lobby'?.2:.17));
  t.x=tx+sx;t.z=tz;
  if(por)t.y=ty-d*.2;
  lobby.sx=sx;lobby.boostOn=boost;lobby.trailOn=trail;
  if(yaw!==null){lobby.yawT=yaw;lobby.auto=false;}else lobby.auto=true;
}
function lobbyCamera(dt){
  const c=lobby.cam,t=lobby.tgt,k=dampT(4.5,dt);
  for(const f of ['x','y','z','d','h'])c[f]+=(t[f]-c[f])*k;
  camPos.set(c.x,c.y+c.h,c.z+c.d);camLook.set(c.x,c.y,c.z);
}
function lobbyEnter(ctx){
  lobby.ctx=ctx;lobbyFocus(ctx==='lobby'?'car':lobby.cat);
  {const inL=ctx==='lobby';if(lobby.hall)lobby.hall.forEach(m=>{m.visible=!inL;});if(lobby.stad)lobby.stad.visible=inL;
   lobbyScene.background.setHex(inL?0x0a1230:0x060914);lobbyScene.fog.color.setHex(inL?0x0a1230:0x060914);
   if(lobby.arch){if(inL){lobby.arch.position.set(30,0,-30);lobby.arch.rotation.y=-.5;}else{lobby.arch.position.set(0,0,-24);lobby.arch.rotation.y=0;}}}
  lobbyMates(ctx==='lobby'?PARTY.map(f=>f.name):[]);
  lobby.mates.forEach(m=>{m.visible=false;});
  if(lobby.arch)lobby.arch.visible=true;
}
function lobbyUpdate(dt){
  lobby.t+=dt;
  if(lobby.auto)lobby.yawT+=dt*.4;
  lobby.yaw+=(lobby.yawT-lobby.yaw)*dampT(6,dt);
  const car=lobby.car;
  if(car){
    car.rotation.y=lobby.yaw;car.position.y=1.15+Math.sin(lobby.t*1.6)*.03;
    const co=lobby.cobj;co.fwd.set(-Math.sin(lobby.yaw),0,-Math.cos(lobby.yaw));co.right.set(Math.cos(lobby.yaw),0,-Math.sin(lobby.yaw));
    spinTick(car,dt);
    const ud=car.userData;
    for(const w of ud.wheels)w.spin.rotation.x+=dt*(lobby.trailOn?10:.6);
    const on=lobby.boostOn;ud.flame.visible=on;ud.flame2.visible=on;
    if(on){const f=.75+rnd()*.5;ud.flame.scale.set(1,1,f);ud.flame2.scale.set(1,1,f);boostEmit(co,false);}
    if(lobby.trailOn)trailEmit(co);
  }
  lobby.mates.forEach(g=>{const c=g.userData.car;c.rotation.y+=dt*.5;spinTick(c,dt);});
  // poussière
  const pa=lobby.dust.geometry.attributes.position;
  for(let i=0;i<pa.count;i++){let y=pa.getY(i)+dt*(.4+(i%5)*.12);if(y>22)y=0;pa.setY(i,y);}
  pa.needsUpdate=true;
  thumbPump();
}
/* glisser pour faire tourner la voiture dans le casier */
{
  let lx=0;
  canvas.addEventListener('pointerdown',e=>{if(phase!=='menu'||lobby.ctx==='lobby')return;lobby.drag=true;lx=e.clientX;lobby.auto=false;});
  window.addEventListener('pointermove',e=>{if(!lobby.drag)return;lobby.yawT+=(e.clientX-lx)*.012;lx=e.clientX;});
  window.addEventListener('pointerup',()=>{lobby.drag=false;});
}

/* =====================  MINIATURES  ===================== */
const TH={r:null,sc:null,cam:null,q:[],cache:{},fail:false};
function thInit(){
  if(TH.r||TH.fail)return;
  try{
    const cv=document.createElement('canvas');cv.width=200;cv.height=124;
    TH.r=new T.WebGLRenderer({canvas:cv,antialias:true,alpha:true,preserveDrawingBuffer:true});
    TH.r.setSize(200,124,false);TH.r.setPixelRatio(1);TH.r.setClearColor(0x000000,0);
    TH.sc=new T.Scene();
    TH.sc.add(new T.HemisphereLight(0xdfe9ff,0x202838,1.05));
    const d1=new T.DirectionalLight(0xffffff,.95);d1.position.set(5,8,6);TH.sc.add(d1);
    const d2=new T.DirectionalLight(0x88aaff,.45);d2.position.set(-6,3,-4);TH.sc.add(d2);
    try{
      const [c,x]=mkc(512,256),g=x.createLinearGradient(0,0,0,256);
      g.addColorStop(0,'#bcd6ff');g.addColorStop(.45,'#2b4f9a');g.addColorStop(.5,'#101a38');g.addColorStop(1,'#05070f');
      x.fillStyle=g;x.fillRect(0,0,512,256);x.fillStyle='#fff';for(let i=0;i<14;i++)x.fillRect(rnd()*512,20+rnd()*80,30+rnd()*60,7);
      const et=new T.CanvasTexture(c);et.mapping=T.EquirectangularReflectionMapping;
      const pm=new T.PMREMGenerator(TH.r);TH.sc.environment=pm.fromEquirectangular(et).texture;pm.dispose();
    }catch(e){}
    TH.cam=new T.PerspectiveCamera(30,200/124,.1,100);
  }catch(e){TH.fail=true;TH.r=null;}
}
function thRender(obj,pos,look){
  TH.sc.add(obj);TH.cam.position.set(pos[0],pos[1],pos[2]);TH.cam.lookAt(look[0],look[1],look[2]);
  TH.r.render(TH.sc,TH.cam);const url=TH.r.domElement.toDataURL('image/png');TH.sc.remove(obj);return url;
}
function th3d(it){
  const acc=0xffd400,prim=hexN(COLORS[(it.i*7)%COLORS.length][1]);
  switch(it.cat){
    case 'car':{const L=Object.assign(defLoad(),{car:it.i,c1:(it.i*7)%COLORS.length,c2:7});const m=makeCarMesh(L,0,true);m.rotation.y=-.75;return thRender(m,[6.6,2.9,6.8],[0,.2,0]);}
    case 'decal':{const L=Object.assign(defLoad(),{car:1,decal:it.i,c1:0,c2:7});const m=makeCarMesh(L,0,true);m.rotation.y=-Math.PI/2+.45;return thRender(m,[6.4,2.4,7],[0,0,0]);}
    case 'fin':{const L=Object.assign(defLoad(),{car:4,fin:it.i,c1:9,c2:7});const m=makeCarMesh(L,0,true);m.rotation.y=-.75;return thRender(m,[6.6,2.9,6.8],[0,.2,0]);}
    case 'wheel':{const g=wheelGroup(it.wd,.55,acc);g.rotation.y=-.5;return thRender(g,[3.6,.5,2.2],[0,0,0]);}
    case 'topper':{const g=TOPFN[it.i]?TOPFN[it.i](acc,prim):new T.Group();g.scale.setScalar(2);g.position.y=-.5;return thRender(g,[2.8,1.2,3.6],[0,.2,0]);}
    case 'ant':{const g=ANTFN[it.i]?ANTFN[it.i](acc,prim):new T.Group();g.scale.setScalar(1.5);g.position.y=-.7;return thRender(g,[2.6,1,3.4],[0,.1,0]);}
  }
  return null;
}
function th2d(it){
  const [c,x]=mkc(200,124);
  const bg=x.createLinearGradient(0,0,0,124);bg.addColorStop(0,'#16213f');bg.addColorStop(1,'#0a1022');x.fillStyle=bg;x.fillRect(0,0,200,124);
  const css=n=>cssRGB(n).map(v=>Math.round(v*255)).join(',');
  if(it.cat==='boost'){
    const b=it.bd;let g=x.createLinearGradient(30,62,170,62);g.addColorStop(0,'rgba('+css(b.c2)+',1)');g.addColorStop(.5,'rgba('+css(b.c1)+',.9)');g.addColorStop(1,'rgba('+css(b.c1)+',0)');
    x.fillStyle=g;x.beginPath();x.moveTo(30,50);x.quadraticCurveTo(110,30,185,62);x.quadraticCurveTo(110,94,30,74);x.closePath();x.fill();
    const rg=mulberry(it.i*13+1);for(let k=0;k<30;k++){x.fillStyle='rgba('+css(rg()<.5?b.c1:b.c2)+','+(.3+rg()*.6)+')';const s=1+rg()*3;x.fillRect(60+rg()*130,35+rg()*54,s,s);}
    x.fillStyle='#9aa5b8';x.fillRect(10,48,24,28);
  }else if(it.cat==='expl'){
    const e=it.ed,cols=e.col.map(s=>s==='t'?'#4a9bff':s),rg=mulberry(it.i*7+3);
    for(let k=0;k<44;k++){const a=rg()*6.283,r=14+rg()*48;x.fillStyle=cols[k%cols.length];x.globalAlpha=.4+rg()*.6;const s=2+rg()*3;x.fillRect(100+Math.cos(a)*r,62+Math.sin(a)*r*.8,s,s);}
    x.globalAlpha=1;x.strokeStyle=cols[0];x.lineWidth=3;x.beginPath();x.arc(100,62,(e.pat==='ring'||e.pat==='nova')?46:20,0,7);x.stroke();
    const rgd=x.createRadialGradient(100,62,0,100,62,34);rgd.addColorStop(0,'rgba(255,255,255,.9)');rgd.addColorStop(1,'rgba(255,255,255,0)');x.fillStyle=rgd;x.fillRect(60,22,80,80);
  }else if(it.cat==='trail'){
    const t=it.td;x.lineCap='round';
    if(t.k==='none'){x.strokeStyle='#445';x.lineWidth=4;x.beginPath();x.moveTo(40,40);x.lineTo(160,84);x.stroke();}
    else for(let k=0;k<26;k++){
      const col=t.col.length?t.col[k%t.col.length]:null,hue=(k*14)%360;
      x.fillStyle=col?col:'hsl('+hue+',90%,60%)';x.globalAlpha=.25+k/34;
      x.beginPath();x.arc(20+k*6.5,88-Math.sin(k*.35)*18-k*.6,3+k*.1,0,7);x.fill();
    }
    x.globalAlpha=1;
  }else if(it.cat==='eng'){
    x.strokeStyle='#2f8bff';x.lineWidth=3;x.beginPath();
    const f=it.ad.base/22;for(let i=0;i<=180;i++){const y=62+Math.sin(i*.06*f)*18*(1-Math.abs(i-90)/130);i?x.lineTo(10+i,y):x.moveTo(10+i,y);}x.stroke();
    x.font='28px system-ui';x.fillStyle='#fff';x.fillText('🔊',84,112);
  }else if(it.cat==='anth'){
    x.fillStyle='#ffd166';const nd=it.nd;nd.forEach((n,i)=>{const hgt=20+((n[0]-200)/1400)*70;x.fillRect(20+i*(150/nd.length),100-hgt,150/nd.length-6,hgt);});
  }
  return c.toDataURL('image/png');
}
function thumbReq(it,img){
  if(TH.cache[it.id]){img.src=TH.cache[it.id];return;}
  TH.q.push({it:it,img:img});
}
function thumbPump(){
  let n=0;
  while(TH.q.length&&n<2){
    const j=TH.q.shift();
    if(!j.img.isConnected&&!j.img.parentNode)continue;
    let url=TH.cache[j.it.id];
    if(!url){
      try{
        if(['car','decal','fin','wheel','topper','ant'].indexOf(j.it.cat)>=0){thInit();url=TH.r?th3d(j.it):null;}
        else url=th2d(j.it);
      }catch(e){url=null;}
      if(!url){const [c,x]=mkc(200,124);x.fillStyle='#16213f';x.fillRect(0,0,200,124);x.fillStyle='#6db0ff';x.font='bold 44px system-ui';x.textAlign='center';x.fillText((j.it.n||'?')[0],100,78);url=c.toDataURL('image/png');}
      TH.cache[j.it.id]=url;
    }
    j.img.src=url;n++;
  }
}

/* =====================================================================
   INTERFACE : lobby, casier, boutique, passe, défis, réglages, tournoi
   ===================================================================== */
const esc=s=>String(s).replace(/[&<>"]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]));
const MODES=[
  {k:'casual',n:'Soccar',ic:'⚽',d:'Soccar en 1v1, 2v2 ou 3v3. Sans classement.'},
  {k:'arc0',n:'Arcade A',ic:'🕹️',d:''},
  {k:'arc1',n:'Arcade B',ic:'🕹️',d:''},
  {k:'big',n:'Grand groupe',ic:'🎉',d:'4 vs 4 Quads : la grosse partie entre amis, sans classement.'},
  {k:'ranked',n:'Soccar classé',ic:'🏆',d:'Duel, Doubles ou Standard : grimpe de Bronze à Supersonic Legend.'},
  {k:'rk_hoops',n:'Hoops classé',ic:'🏀',d:'Le basket en voiture, en classé.'},
  {k:'rk_rumble',n:'Rumble classé',ic:'⚡',d:'Power-ups en classé.'},
  {k:'rk_drop',n:'Dropshot classé',ic:'⬡',d:'Brise le sol adverse, en classé.'},
  {k:'rk_snow',n:'Snow Day classé',ic:'🏒',d:'Hockey sur glace, en classé.'},
  {k:'rk_heat',n:'Heatseeker classé',ic:'🎯',d:'Balle à tête chercheuse, en classé.'},
  {k:'hoops',n:'Hoops',ic:'🏀',d:'Fais passer la balle dans le panier, du basket en voiture.'},
  {k:'rumble',n:'Rumble',ic:'⚡',d:'Soccar avec power-ups : aimant, pointes, grappin, tornade…'},
  {k:'drop',n:'Dropshot',ic:'⬡',d:'Charge la balle, brise le sol adverse et marque dans le trou.'},
  {k:'snow',n:'Snow Day',ic:'🏒',d:'Hockey sur glace : un palet à la place de la balle.'},
  {k:'heat',n:'Heatseeker',ic:'🎯',d:'La balle cherche le but et accélère à chaque frappe.'},
  {k:'free',n:'Entraînement libre',ic:'🛠️',d:'Arène vide, boost illimité. R = remettre la balle.'},
  {k:'train',n:'Packs d\'entraînement',ic:'🎓',d:'Tirs, arrêts, aériens : enchaîne les scénarios.'},
  {k:'moves',n:'Pack mécaniques',ic:'🧪',d:'Un coach te montre le move (flip reset, air dribble, double tap, psycho, pinchs…), puis tu t\'entraînes.'},
  {k:'exhib',n:'Exhibition',ic:'🎮',d:'Un match contre des bots, tout de suite, sans file d\'attente. Choisis la playlist.'},
  {k:'season',n:'Mode saison',ic:'📅',d:'Un championnat de 7 matchs contre 7 équipes. Classement et récompense finale.'},
  {k:'workshop',n:'Cartes Workshop',ic:'🧩',d:'Des cartes à mutateurs prêtes à jouer : Lune, Balle géante, Flipper…'},
  {k:'comm',n:'Cartes de la communauté',ic:'🌍',d:'Joue les cartes créées par les autres joueurs (en ligne). Crée la tienne dans CRÉATEUR et publie-la.'},
  {k:'tour',n:'Tournoi',ic:'🥇',d:'Bracket à 8 équipes en 3 tours, gros gains à la clé.'},
  {k:'custom',n:'Match privé',ic:'🔧',d:'Mutateurs : gravité, balle, boost, vitesse, démos, arène…'}
];
const SZ={casual:[1,2,3],big:[4],exhib:[1,2,3,4],ranked:[1,2,3],rk_hoops:[2],rk_rumble:[3],rk_drop:[3],rk_snow:[3],rk_heat:[2],hoops:[1,2,3],rumble:[2,3],drop:[2,3],snow:[1,2,3],heat:[1,2,3],custom:[1,2,3,4],comm:[1,2,3,4]};
const SZN={1:'1 vs 1',2:'2 vs 2',3:'3 vs 3',4:'4 vs 4 Quads'};
function modeSize(k){
  const a=SZ[k];if(!a)return k==='tour'?2:1;
  const s=SV.menu.size;return a.indexOf(s)>=0?s:a[Math.min(a.length-1,1)];
}
const UI={home:true,scr:'lobby',cat:'car',sel:null,shop:'feat',cf:-1,own:false,cr:'car'};
const $scr=$('scr');
function toast(msg){const t=$('toast');t.textContent=msg;t.classList.add('show');clearTimeout(toast.h);toast.h=setTimeout(()=>t.classList.remove('show'),2200);}
function modal(html,wide){$('mbox').innerHTML=html;$('mbox').parentNode.classList.add('show');}
function modalClose(){$('modal').classList.remove('show');}
/* transitions : « enter » rejoue l'arrivée des éléments de l'accueil, « out » fait disparaître le menu en fondu avant de le masquer */
function uiEnter(){const u=$('ui');u.classList.remove('enter');void u.offsetWidth;u.classList.add('enter');clearTimeout(uiEnter.t);uiEnter.t=setTimeout(()=>u.classList.remove('enter'),900);}
function curtain(ms){const c=$('curtain');if(!c)return;c.style.transition='none';c.style.opacity='1';void c.offsetWidth;c.style.transition='opacity '+(ms||650)+'ms ease';c.style.opacity='0';}
function showUI(){const u=$('ui');clearTimeout(hideUI.t);u.classList.remove('out');u.classList.add('show');uiEnter();}
function hideUI(){const u=$('ui');clearTimeout(hideUI.t);if(u.classList.contains('show')&&!u.classList.contains('out')){u.classList.add('out');hideUI.t=setTimeout(()=>u.classList.remove('show','out','enter'),320);}$('queue').classList.remove('show');modalClose();}

/* ---------- barre du haut ---------- */
const NAV=[['lobby','JOUER'],['shop','BOUTIQUE'],['locker','GARAGE'],['prof','PROFIL'],['lb','CLASSEMENT'],['acc','COMPTE'],['maker','CRÉATEUR'],['set','OPTIONS'],['quit','QUITTER']];
function refreshTop(){
  $('pname').textContent=SV.name||'Joueur';
  $('ptitle').textContent=IT['title:'+SV.load.title].n;
  $('prof').style.background=IT['ban:'+SV.load.ban].css;
  const lv=lvlOf(SV.xp);$('lvl').textContent=lv;$('mtitle').textContent=MUS.name?('♪ '+MUS.name):'Musique du menu';
  $('xpbar').style.width=((SV.xp%500)/5)+'%';
  $('cred').textContent='💰 '+SV.credits.toLocaleString('fr-FR');
  $('daily').classList.toggle('pulse',SV.daily.last!==todayKey());
  {const home=!!UI.home&&UI.scr==='lobby';if(home){try{chSync();$('chw').innerHTML='<b>DÉFIS</b>'+chToday().map(c=>{const pr=Math.min(c.v,SV.ch.prog[c.id]||0);return '<div class="cw"><span>'+esc(c.n.replace('{n}',c.v))+'</span><div class="bar"><i style="width:'+(pr/c.v*100)+'%"></i></div><small>'+pr+' / '+c.v+'</small></div>';}).join('');}catch(e){}}$('ui').classList.toggle('home',home);$('back').classList.toggle('hide',home);partyUiSync(home);}
  $('nav').innerHTML=NAV.map(n=>'<button class="nb'+(UI.scr===n[0]||(n[0]==='lobby'&&(UI.scr==='tour'||UI.scr==='season'||UI.scr==='comm'))?' on':'')+'" data-go="'+n[0]+'">'+n[1]+'</button>').join('');
}
function goHome(){UI.home=true;go('lobby');}
function quitGame(){
  modal('<div class=\"panel small\"><h2>QUITTER ?</h2><button class=\"go\" id=\"qy\">Quitter</button><button class=\"go alt\" id=\"qn\">Annuler</button></div>');
  $('qn').onclick=modalClose;$('qy').onclick=()=>{modalClose();try{window.close();}catch(e){}toast('Ferme l\'onglet pour quitter le jeu.');};
}
function rProf(){
  const lv=lvlOf(SV.xp);
  $scr.innerHTML='<div class=\"cpanel wide\"><h2>PROFIL</h2><div class=\"sub\">'+esc(SV.name||'Joueur')+' • Niveau '+lv+' • '+((IT['title:'+SV.load.title]||{}).n||'')+'</div><h3>CLASSEMENT</h3><div class=\"ranks\">'+
    MODES.filter(x=>isRk(x.k)).map(x=>SZ[x.k].map(z=>{const kk=rkKey(x.k,z),rk=rankInfo(kk),mm=mmrOf(kk);return '<div class=\"rank\" style=\"border-color:'+rk.c+'\"><span style=\"color:'+rk.c+'\">'+rk.n.toUpperCase()+'</span><small>'+x.n.replace(' classé','')+' • '+SZN[z]+' • '+(rk.un?(PLACE_N-rk.left)+' / '+PLACE_N+' placement':mm+' MMR')+'</small></div>';}).join('')).join('')+'</div></div>';
}
/* ---------- STAFF & BANNISSEMENTS ----------
   Le compte staff (e-mail vérifié) peut bannir des joueurs et supprimer des cartes ; ses cartes publiées sont « officielles ».
   Collection Firestore « bans » : un document par identifiant de joueur. ATTENTION : ce contrôle côté page n'est qu'un confort,
   la vraie protection est dans les règles Firestore (voir firestore-staff.rules.txt). */
const STAFF_EMAIL='college.maxence59@gmail.com';
const BAN={col:'bans',cache:null,t:0,me:false,meId:'',meT:0,
  isStaff(){let u=null;try{u=CLOUD.user;}catch(e){}return !!(u&&u.emailVerified&&String(u.email||'').toLowerCase()===STAFF_EMAIL);},
  async list(force){const F=window.RA_FB;if(!F)return {};
    if(!force&&BAN.cache&&Date.now()-BAN.t<60000)return BAN.cache;
    try{const sn=await F.getDocs(F.query(F.collection(F.db,BAN.col),F.limit(500)));const o={};sn.docs.forEach(d=>{o[d.id]=d.data();});BAN.cache=o;BAN.t=Date.now();}
    catch(e){console.warn('Bannissements',e);BAN.cache=BAN.cache||{};}
    return BAN.cache;},
  async mine(){const F=window.RA_FB,id=LB.getId();if(!F)return false;
    if(BAN.meId===id&&Date.now()-BAN.meT<60000)return BAN.me;
    try{const d=await F.getDoc(F.doc(F.db,BAN.col,id));BAN.me=d.exists();}catch(e){BAN.me=false;}
    BAN.meId=id;BAN.meT=Date.now();return BAN.me;},
  async ban(id,name){const F=window.RA_FB;if(!BAN.isStaff()||!F||!id)return false;
    if(id===LB.getId()){toast('Tu ne peux pas te bannir toi-même');return false;}
    try{await F.setDoc(F.doc(F.db,BAN.col,String(id)),{name:String(name||'?').slice(0,24),by:CLOUD.user.uid,ts:F.serverTimestamp()});
      BAN.cache=null;LB.cache={};COMM.cache={};toast('🔨 '+(name||'Joueur')+' est banni');return true;}
    catch(e){console.warn('Ban',e);toast('Bannissement impossible (règles Firestore ?)');return false;}},
  async unban(id){const F=window.RA_FB;if(!BAN.isStaff()||!F)return false;
    try{await F.deleteDoc(F.doc(F.db,BAN.col,String(id)));BAN.cache=null;LB.cache={};COMM.cache={};toast('Joueur débanni');return true;}
    catch(e){console.warn('Déban',e);toast('Action impossible (règles Firestore ?)');return false;}},
  async panel(){const bn=await BAN.list(true),ids=Object.keys(bn);
    modal('<div class="panel small"><h2>🛡️ JOUEURS BANNIS</h2>'+(ids.length?ids.map(id=>'<div class="row" style="display:flex;gap:6px;margin:6px 0;align-items:center"><span style="flex:1;text-align:left">'+esc(bn[id].name||'?')+'</span><button class="go alt" style="margin:0;width:auto;padding:6px 12px" data-ub="'+esc(id)+'">Débannir</button></div>').join(''):'<div class="lbmsg">Aucun joueur banni.</div>')+'<button class="go alt" id="banX">Fermer</button></div>');
    $('banX').onclick=modalClose;
    $('mbox').querySelectorAll('[data-ub]').forEach(b=>b.onclick=async()=>{if(await BAN.unban(b.dataset.ub)){modalClose();BAN.panel();}});}
};
/* ---------- CLASSEMENT EN LIGNE (Firebase / Firestore) ---------- */
const LB={
  col:'players',timer:0,last:'',tries:0,cache:{},
  BOARDS:[['xp','NIVEAU'],['wins','VICTOIRES'],['goals','BUTS'],['saves','ARRÊTS']],
  /* classements classés : un par playlist et par format (id « rk:<clé> », champ Firestore rk.m_<clé>) */
  rkList(){const a=[];MODES.filter(x=>isRk(x.k)).forEach(x=>SZ[x.k].forEach(z=>{const k=rkKey(x.k,z);a.push({key:k,mk:'m_'+k,label:x.n.replace(' classé','')+' '+z+'v'+z});}));return a;},
  defRk(){const e=LB.rkList().find(x=>((SV.plc&&SV.plc[x.key])||0)>=PLACE_N);return e?e.key:'2';},
  isRkB(b){return String(b).indexOf('rk:')===0;},
  field(b){return LB.isRkB(b)?'rk.m_'+b.slice(3):b;},
  val(o,b){return LB.isRkB(b)?(+((o.rk||{})['m_'+b.slice(3)])||0):(+o[b]||0);},
  label(b){
    if(LB.isRkB(b)){const e=LB.rkList().find(x=>x.key===b.slice(3));return e?e.label:'Classé';}
    return (LB.BOARDS.find(x=>x[0]===b)||LB.BOARDS[0])[1];
  },
  getId(){let u=null;try{u=CLOUD.user;}catch(e){}return u?u.uid:LB.guestId();},
  guestId(){
    let id='';try{id=localStorage.getItem('rocketArena3D.lbId')||'';}catch(e){}
    if(!/^[A-Za-z0-9]{24}$/.test(id)){
      const a='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';id='';
      const r=new Uint8Array(24);(window.crypto||{getRandomValues:x=>{for(let i=0;i<x.length;i++)x[i]=Math.random()*256;}}).getRandomValues(r);
      for(let i=0;i<r.length;i++)id+=a[r[i]%a.length];
      try{localStorage.setItem('rocketArena3D.lbId',id);}catch(e){}
    }
    return id;
  },
  bestMmr(){let b=0;for(const k in SV.mmr){if(((SV.plc&&SV.plc[k])||0)>=PLACE_N&&SV.mmr[k]>b)b=SV.mmr[k];}return Math.round(b);},
  payload(){
    const S=SV.stats,n=v=>Math.max(0,Math.round(+v||0)),rk={};
    LB.rkList().forEach(e=>{if(((SV.plc&&SV.plc[e.key])||0)>=PLACE_N)rk[e.mk]=n(mmrOf(e.key));});   /* seulement les playlists dont le placement est terminé */
    return {name:String(SV.name||'Joueur').trim().slice(0,12)||'Joueur',xp:n(SV.xp),wins:n(S.wins),goals:n(S.goals),assists:n(S.assists),saves:n(S.saves),demos:n(S.demos),matches:n(S.matches),mmr:LB.bestMmr(),rk:rk};
  },
  schedule(){clearTimeout(LB.timer);LB.timer=setTimeout(LB.push,3500);},
  async push(){
    const F=window.RA_FB;
    if(!F){if(!window.RA_FB_ERR&&LB.tries++<8)LB.timer=setTimeout(LB.push,2000);return;}
    const p=LB.payload();if(!p.matches)return;
    if(await BAN.mine())return; /* joueur banni : plus d'envoi au classement */
    const sig=JSON.stringify(p);if(sig===LB.last)return;
    try{
      await F.setDoc(F.doc(F.db,LB.col,LB.getId()),Object.assign({},p,{updated:F.serverTimestamp()}));
      LB.last=sig;LB.cache={};
    }catch(e){console.warn('Classement : envoi impossible',e);}
  },
  async fetch(b,force){
    const F=window.RA_FB;if(!F)throw new Error('firebase');
    const c=LB.cache[b];if(!force&&c&&Date.now()-c.t<20000)return c;
    const col=F.collection(F.db,LB.col);
    const fld=LB.field(b);
    const sn=await F.getDocs(F.query(col,F.orderBy(fld,'desc'),F.limit(50)));
    const bn=await BAN.list();
    const rows=sn.docs.map(d=>Object.assign({id:d.id},d.data())).filter(r=>LB.val(r,b)>0&&!bn[r.id]);
    const me=LB.getId(),mine=LB.payload(),mv=LB.val(mine,b);let pos=0;
    const idx=rows.findIndex(r=>r.id===me);
    if(idx>=0)pos=idx+1;
    else if(mine.matches&&mv>0){
      try{const ct=await F.getCountFromServer(F.query(col,F.where(fld,'>',mv)));pos=ct.data().count+1;}catch(e){}
    }
    return LB.cache[b]={t:Date.now(),rows:rows,pos:pos};
  },
  fmt(b,r){
    if(b==='xp')return 'Niv. '+lvlOf(r.xp)+'<small>'+(+r.xp).toLocaleString('fr-FR')+' XP</small>';
    if(LB.isRkB(b)){const v=LB.val(r,b),rk=rankOf(v);return '<span style="color:'+rk.c+'">'+esc(rk.n)+'</span><small>'+v+' MMR</small>';}
    return (+r[b]).toLocaleString('fr-FR');
  }
};
function rLb(){
  const b=UI.lb||'xp',isR=LB.isRkB(b),lab=LB.label(b);
  if(isR)UI.lbRk=b.slice(3);
  $scr.innerHTML='<div class="cpanel wide"><h2>CLASSEMENT</h2><div class="sub">Meilleurs joueurs en ligne</div>'+
    '<div class="tabs">'+LB.BOARDS.map(x=>'<button class="tab'+(x[0]===b?' on':'')+'" data-lb="'+x[0]+'">'+x[1]+'</button>').join('')+
    '<button class="tab'+(isR?' on':'')+'" data-lb="rk:'+(UI.lbRk||LB.defRk())+'">CLASSÉ</button></div>'+
    (isR?'<div class="frow" style="padding:10px 2px 0">'+LB.rkList().map(e=>'<button class="chip2'+('rk:'+e.key===b?' on':'')+'" data-lb="rk:'+e.key+'">'+esc(e.label)+'</button>').join('')+'</div>':'')+
    '<div class="grp"><b>TON PSEUDO</b><input id="lbName" maxlength="12" value="'+esc(SV.name||'')+'" class="txt"></div>'+
    '<div id="lbBody"><div class="lbmsg">Chargement…</div></div>'+
    '<button class="go alt" data-lb="refresh">Actualiser</button></div>';
  $('lbName').oninput=e=>{SV.name=e.target.value.slice(0,12);saveSV();refreshTop();};
  const stale=()=>UI.scr!=='lb'||(UI.lb||'xp')!==b||!$('lbBody');
  LB.fetch(b,UI.lbForce).then(d=>{
    if(stale())return;
    const me=LB.getId(),mine=LB.payload();
    let h='';
    if(!mine.matches)h+='<div class="lbme">Joue un match pour apparaître dans le classement.</div>';
    else if(LB.val(mine,b)>0)h+='<div class="lbme">Ta place : '+(d.pos?'#'+d.pos:'—')+' • '+LB.fmt(b,mine)+'</div>';
    else if(isR)h+='<div class="lbme">Termine tes '+PLACE_N+' matchs de placement en « '+esc(lab)+' » pour apparaître ('+Math.min(PLACE_N,(SV.plc&&SV.plc[b.slice(3)])||0)+' / '+PLACE_N+').</div>';
    else h+='<div class="lbme">Pas encore classé dans « '+lab+' ».</div>';
    if(!d.rows.length)h+='<div class="lbmsg">Personne dans ce classement pour l\'instant. Sois le premier !</div>';
    else h+='<table class="lbt"><tr><th>#</th><th>JOUEUR</th><th style="text-align:right">'+(isR?'RANG':lab)+'</th></tr>'+d.rows.map((r,i)=>
      '<tr class="'+(r.id===me?'me ':'')+(i<3?'t'+(i+1):'')+'"><td class="n">'+(i+1)+'</td><td>'+esc(r.name||'Joueur')+(BAN.isStaff()&&r.id!==me?' <button class="opt" data-bn="'+i+'" title="Bannir" style="padding:2px 8px">🔨</button>':'')+'</td><td class="v">'+LB.fmt(b,r)+'</td></tr>').join('')+'</table>';
    $('lbBody').innerHTML=h;
    $('lbBody').querySelectorAll('[data-bn]').forEach(bt=>bt.onclick=async()=>{const r=d.rows[+bt.dataset.bn];if(!confirm('Bannir '+(r.name||'ce joueur')+' ?'))return;if(await BAN.ban(r.id,r.name)){UI.lbForce=true;rLb();}});
  }).catch(e=>{
    if(stale())return;console.warn('Classement',e);
    const perm=e&&(e.code==='permission-denied'||/permission/i.test(e.message||''));
    $('lbBody').innerHTML='<div class="lbmsg">'+(perm?'Accès refusé : publie les règles Firestore (voir firestore.rules).':'Classement indisponible. Vérifie ta connexion puis appuie sur Actualiser.')+'</div>';
  });
  UI.lbForce=false;
}
LB.schedule(); /* synchronise la progression existante au lancement */

/* ---------- COMPTES (Firebase Auth) + SAUVEGARDE CLOUD ---------- */
const CLOUD={
  user:null,blocked:false,busy:false,pushing:false,timer:0,lastJson:'',st:'',stAt:0,stMsg:'',mail:'',msg:'',
  META_KEY:'rocketArena3D.cloudMeta',
  meta(){try{return JSON.parse(localStorage.getItem(CLOUD.META_KEY)||'{}')||{};}catch(e){return {};}},
  setMeta(m){try{localStorage.setItem(CLOUD.META_KEY,JSON.stringify(m));}catch(e){}},
  /* empreinte « progression » : sert à savoir si cet appareil a avancé depuis la dernière synchro */
  sig(sv){sv=sv||SV;return [sv.xp,(sv.stats||{}).matches,sv.credits,Object.keys(sv.owned||{}).length].join('|');},
  rev(){return Date.now().toString(36)+Math.random().toString(36).slice(2,10);},
  pristine(sv){return !(+sv.xp)&&!((sv.stats||{}).matches);},
  schedule(){clearTimeout(CLOUD.timer);if(!CLOUD.user)return;CLOUD.timer=setTimeout(()=>CLOUD.push(),6000);},
  init(){
    const F=window.RA_FB;if(!F||!F.auth)return;
    let first=true;
    F.onAuthStateChanged(F.auth,u=>{CLOUD.onUser(u);if(first){first=false;if(!u)GATE.show();}}); /* écran de connexion seulement au lancement et seulement sans compte connecté */
    document.addEventListener('visibilitychange',()=>{if(document.hidden)CLOUD.push();});
  },
  onUser(u){
    CLOUD.user=u||null;if(u)GATE.hide();CLOUD.blocked=false;CLOUD.lastJson='';CLOUD.st='';CLOUD.stMsg='';
    LB.last='';LB.cache={};BAN.meT=0;BAN.cache=null;LB.schedule();
    if(UI.scr==='acc')rAcc();
    if(u)CLOUD.sync();
  },
  whenMenu(fn){const w=()=>{if(phase==='menu')fn();else setTimeout(w,1500);};w();},
  pull(cloud){
    let ok=false;try{JSON.parse(cloud.data);ok=true;}catch(e){}
    if(!ok||!CLOUD.user){toast('Sauvegarde du compte illisible');CLOUD.blocked=false;return;}
    try{localStorage.setItem(SAVE_KEY,cloud.data);}catch(e){}
    SV=loadSV();
    CLOUD.setMeta({uid:CLOUD.user.uid,rev:cloud.rev,sig:CLOUD.sig(SV)});
    toast('Progression du compte chargée…');
    setTimeout(()=>location.reload(),900);
  },
  ask(cloud){
    CLOUD.whenMenu(()=>{
      if(!CLOUD.user){CLOUD.blocked=false;return;}
      modal('<div class="panel small"><h2>SAUVEGARDE TROUVÉE</h2><div class="sub">La progression de ton compte et celle de cet appareil sont différentes.</div>'+
        '<div class="frow"><div class="finfo"><b>☁️ Compte</b> <small>'+esc(cloud.name||'Joueur')+' • Niveau '+lvlOf(+cloud.xp||0)+' • '+(+cloud.matches||0)+' matchs</small></div></div>'+
        '<div class="frow"><div class="finfo"><b>📱 Cet appareil</b> <small>'+esc(SV.name||'Joueur')+' • Niveau '+lvlOf(+SV.xp||0)+' • '+((SV.stats||{}).matches||0)+' matchs</small></div></div>'+
        '<button class="go" id="cvC">Utiliser le compte</button><button class="go alt" id="cvL">Garder cet appareil (remplace le compte)</button></div>');
      $('cvC').onclick=()=>{modalClose();CLOUD.pull(cloud);};
      $('cvL').onclick=()=>{modalClose();CLOUD.setMeta({uid:CLOUD.user.uid,rev:cloud.rev,sig:''});CLOUD.blocked=false;CLOUD.push(true).then(()=>CLOUD.after());};
    });
  },
  async sync(){
    const F=window.RA_FB,u=CLOUD.user;if(!F||!u)return;
    CLOUD.blocked=true;CLOUD.paint();
    try{
      const snap=await F.getDoc(F.doc(F.db,'saves',u.uid));
      if(CLOUD.user!==u)return;
      const cloud=snap.exists()?snap.data():null,meta=CLOUD.meta(),same=meta.uid===u.uid;
      if(!cloud||typeof cloud.data!=='string'){CLOUD.blocked=false;await CLOUD.push(true);return CLOUD.after();}
      if(same&&cloud.rev===meta.rev){CLOUD.blocked=false;await CLOUD.push();return CLOUD.after();}
      if(same){
        if(CLOUD.sig()===meta.sig){CLOUD.whenMenu(()=>CLOUD.pull(cloud));return;}   /* rien de neuf ici : on récupère le compte */
        return CLOUD.ask(cloud);                                                 /* les deux ont avancé */
      }
      /* première connexion de ce compte sur cet appareil */
      if(CLOUD.pristine(SV)){CLOUD.whenMenu(()=>CLOUD.pull(cloud));return;}
      if((!(+cloud.xp)&&!(+cloud.matches))||(+cloud.xp===+SV.xp&&+cloud.matches===+SV.stats.matches)){
        CLOUD.setMeta({uid:u.uid,rev:cloud.rev,sig:CLOUD.sig()});CLOUD.blocked=false;await CLOUD.push(true);return CLOUD.after();
      }
      CLOUD.ask(cloud);
    }catch(e){CLOUD.blocked=false;CLOUD.st='err';CLOUD.stMsg=CLOUD.err(e);CLOUD.paint();console.warn('Sync compte',e);}
  },
  after(){
    const u=CLOUD.user;if(!u)return;
    if((SV.name||'Joueur')==='Joueur'){
      const n=String(u.displayName||(u.email||'').split('@')[0]||'').trim().slice(0,12);
      if(n){SV.name=n;saveSV();refreshTop();}
    }
    CLOUD.paint();
  },
  async push(force){
    const F=window.RA_FB,u=CLOUD.user;
    if(!F||!u||CLOUD.blocked)return;
    if(CLOUD.pushing){CLOUD.schedule();return;}
    const json=JSON.stringify(SV);
    if(!force&&json===CLOUD.lastJson)return;
    CLOUD.pushing=true;CLOUD.st='saving';CLOUD.paint();
    const ref=F.doc(F.db,'saves',u.uid),rev=CLOUD.rev(),meta=CLOUD.meta(),base=meta.uid===u.uid?meta.rev:null,sig=CLOUD.sig();
    let conflict=false;
    try{
      await F.runTransaction(F.db,async tx=>{
        const s=await tx.get(ref);
        if(s.exists()&&s.data().rev!==base)throw {code:'ra/conflict'};
        tx.set(ref,{data:json,rev:rev,xp:Math.round(+SV.xp||0),matches:Math.round(+SV.stats.matches||0),name:String(SV.name||'Joueur').slice(0,12)});
      });
      CLOUD.setMeta({uid:u.uid,rev:rev,sig:sig});CLOUD.lastJson=json;CLOUD.st='ok';CLOUD.stAt=Date.now();CLOUD.stMsg='';
    }catch(e){
      CLOUD.st='err';CLOUD.stMsg=CLOUD.err(e);conflict=!!(e&&e.code==='ra/conflict');
      if(!conflict)console.warn('Sauvegarde cloud',e);
    }
    CLOUD.pushing=false;CLOUD.paint();
    if(conflict&&!CLOUD.blocked)CLOUD.sync();   /* un autre appareil a sauvegardé entre-temps */
  },
  err(e){
    const c=(e&&e.code)||'';
    return ({
      'ra/conflict':'Un autre appareil a sauvegardé entre-temps.',
      'auth/invalid-email':'Adresse e-mail invalide.',
      'auth/missing-email':'Entre ton adresse e-mail.',
      'auth/missing-password':'Entre ton mot de passe.',
      'auth/weak-password':'Mot de passe trop court (6 caractères minimum).',
      'auth/email-already-in-use':'Un compte existe déjà avec cet e-mail : connecte-toi.',
      'auth/invalid-credential':'E-mail ou mot de passe incorrect.',
      'auth/wrong-password':'E-mail ou mot de passe incorrect.',
      'auth/user-not-found':'E-mail ou mot de passe incorrect.',
      'auth/too-many-requests':'Trop d\'essais : réessaie dans quelques minutes.',
      'auth/network-request-failed':'Pas de connexion internet.',
      'auth/popup-closed-by-user':'Fenêtre Google fermée.',
      'auth/cancelled-popup-request':'Fenêtre Google fermée.',
      'auth/unauthorized-domain':'Domaine non autorisé : ajoute-le dans Firebase → Authentication → Paramètres → Domaines autorisés.',
      'auth/operation-not-allowed':'Méthode de connexion désactivée : active-la dans Firebase → Authentication → Sign-in method.',
      'auth/operation-not-supported-in-this-environment':'Connexion impossible ici : ouvre le jeu depuis un site en https (ou localhost).',
      'permission-denied':'Accès refusé : publie les règles Firestore (firestore.rules).',
      'unavailable':'Pas de connexion internet.'
    })[c]||('Erreur : '+(c||(e&&e.message)||'inconnue'));
  },
  statusText(){
    if(CLOUD.blocked)return 'Synchronisation…';
    if(CLOUD.st==='saving')return 'Sauvegarde en cours…';
    if(CLOUD.st==='ok')return '☁️ Sauvegardé dans le cloud à '+new Date(CLOUD.stAt).toLocaleTimeString('fr-FR',{hour:'2-digit',minute:'2-digit'})+'.';
    if(CLOUD.st==='err')return '⚠️ '+CLOUD.stMsg;
    return 'La sauvegarde automatique est active.';
  },
  say(t){CLOUD.msg=t;const el=$('accMsg');if(el)el.textContent=t;},
  paint(){const el=$('accSt');if(el&&UI.scr==='acc')el.textContent=CLOUD.statusText();},
  async act(k){
    const F=window.RA_FB;
    if(!F||!F.auth){CLOUD.say('Comptes indisponibles (Firebase non chargé). Vérifie ta connexion.');return;}
    if(CLOUD.busy)return;
    if(k==='logout'){try{await F.signOut(F.auth);toast('Déconnecté');}catch(e){toast(CLOUD.err(e));}return;}
    if(k==='sync'){
      if(CLOUD.blocked){toast('Synchronisation en cours…');return;}
      await CLOUD.push(true);toast(CLOUD.st==='ok'?'Sauvegardé dans le cloud':'Échec de la sauvegarde');return;
    }
    const mail=String(($('accMail')||{}).value||'').trim(),pass=String(($('accPass')||{}).value||'');
    CLOUD.mail=mail;CLOUD.busy=true;CLOUD.say('…');
    try{
      if(k==='google'){
        const p=new F.GoogleAuthProvider();
        try{await F.signInWithPopup(F.auth,p);}
        catch(e){if(e&&e.code==='auth/popup-blocked'){await F.signInWithRedirect(F.auth,p);return;}throw e;}
      }else if(k==='login'){await F.signInWithEmailAndPassword(F.auth,mail,pass);}
      else if(k==='signup'){await F.createUserWithEmailAndPassword(F.auth,mail,pass);}
      else if(k==='reset'){
        if(!mail){CLOUD.say('Entre ton e-mail puis appuie sur « Mot de passe oublié ».');CLOUD.busy=false;return;}
        await F.sendPasswordResetEmail(F.auth,mail);CLOUD.say('E-mail de réinitialisation envoyé à '+mail+'.');CLOUD.busy=false;return;
      }
      CLOUD.say('');loginFx(k==='signup');
    }catch(e){CLOUD.say(CLOUD.err(e));}
    CLOUD.busy=false;
  }
};
/* ---------- ANIMATION DE CONNEXION : une balle rentre dans un but, puis « Connecté ! » ou « Compte créé ! » ---------- */
function loginFx(created){
  try{
    const old=$('lfx');if(old)old.remove();
    if(!$('lfxCss')){const st=document.createElement('style');st.id='lfxCss';st.textContent=
      '#lfx{position:fixed;inset:0;z-index:90;display:flex;flex-direction:column;align-items:center;justify-content:center;background:radial-gradient(ellipse at 50% 40%,rgba(19,38,90,.94) 0%,rgba(5,8,20,.96) 75%);opacity:0;transition:opacity .25s;cursor:pointer;font-family:var(--font);color:#fff;overflow:hidden}'+
      '#lfx.on{opacity:1}'+
      '#lfx .sc{position:relative;width:min(86vw,460px);height:min(42vh,300px)}'+
      '#lfx .gl{position:absolute;left:15%;width:70%;top:6%;height:56%;border:6px solid #fff;border-bottom:0;border-radius:6px 6px 0 0;box-shadow:0 0 30px rgba(120,170,255,.45);background:'+
        'repeating-linear-gradient(0deg,rgba(255,255,255,.32) 0 1.5px,transparent 1.5px 13px),repeating-linear-gradient(90deg,rgba(255,255,255,.32) 0 1.5px,transparent 1.5px 13px),rgba(80,130,255,.12);transform-origin:50% 100%}'+
      '#lfx .gl.hit{animation:lfNet .55s ease-out}'+
      '@keyframes lfNet{0%{transform:scale(1)}25%{transform:scale(1.07,1.04)}55%{transform:scale(.98)}100%{transform:scale(1)}}'+
      '#lfx .gr{position:absolute;left:6%;right:6%;top:62%;height:3px;background:linear-gradient(90deg,transparent,#7dffb0,transparent);opacity:.7}'+
      '#lfx .bl{position:absolute;left:50%;top:0;width:54px;height:54px;margin:-27px 0 0 -27px;border-radius:50%;'+
        'background:radial-gradient(circle at 34% 30%,#fff 0 18%,#dfe6f5 38%,#8d9bb8 100%);box-shadow:inset -6px -8px 14px rgba(20,30,60,.45),0 0 22px rgba(255,255,255,.55);opacity:0}'+
      '#lfx .bl::after{content:"";position:absolute;inset:14px;border-radius:50%;border:3px solid rgba(40,60,110,.55)}'+
      '#lfx .fl{position:absolute;inset:0;background:#fff;opacity:0;pointer-events:none}'+
      '#lfx .tx{text-align:center;margin-top:18px;opacity:0;transform:scale(.4)}'+
      '#lfx .tx.on{animation:lfPop .6s cubic-bezier(.2,1.5,.4,1) both}'+
      '@keyframes lfPop{0%{opacity:0;transform:scale(.4)}100%{opacity:1;transform:scale(1)}}'+
      '#lfx .tx b{display:block;font-size:clamp(30px,9vmin,64px);font-style:italic;font-weight:900;letter-spacing:3px;line-height:1.05;color:#7dffb0;text-shadow:0 0 22px rgba(125,255,176,.6),0 4px 16px #000}'+
      '#lfx .tx span{display:block;margin-top:8px;font-size:clamp(13px,2.6vmin,18px);letter-spacing:2px;opacity:.85}'+
      '#lfx .pt{position:absolute;left:50%;top:0;width:9px;height:9px;margin:-4px 0 0 -4px;border-radius:2px;pointer-events:none}';
      document.head.appendChild(st);}
    const u=(window.RA_FB&&window.RA_FB.auth&&window.RA_FB.auth.currentUser)||CLOUD.user||null;
    const who=u?(u.displayName||u.email||''):'';
    const o=document.createElement('div');o.id='lfx';
    o.innerHTML='<div class="sc"><div class="gl"></div><div class="gr"></div><div class="bl"></div><div class="fl"></div></div>'+
      '<div class="tx"><b>'+(created?'COMPTE CRÉÉ !':'CONNECTÉ !')+'</b><span>'+(who?esc(who):'BIENVENUE DANS L\'ARÈNE')+'</span></div>';
    document.body.appendChild(o);
    let done=false;
    const close=()=>{if(done)return;done=true;o.classList.remove('on');setTimeout(()=>o.remove(),300);};
    o.onclick=close;
    void o.offsetWidth;o.classList.add('on');
    const sc=o.querySelector('.sc'),bl=o.querySelector('.bl'),gl=o.querySelector('.gl'),tx=o.querySelector('.tx'),fl=o.querySelector('.fl');
    const W=sc.offsetWidth,H=sc.offsetHeight,gy=H*.36;   /* gy : fond du filet, là où la balle s'arrête */
    const cols=['#7dffb0','#ffd166','#6db0ff','#ff8a2a','#ffffff','#ff4fd8'];
    const burst=()=>{
      for(let i=0;i<34;i++){
        const d=document.createElement('i');d.className='pt';d.style.background=cols[i%cols.length];d.style.top=gy+'px';
        sc.appendChild(d);
        const a=Math.random()*6.283,r=60+Math.random()*Math.min(W,260)*.75;
        d.animate([{transform:'translate(0,0) scale(1)',opacity:1},{transform:'translate('+Math.cos(a)*r+'px,'+(Math.sin(a)*r-30)+'px) scale(.2) rotate('+(Math.random()*720)+'deg)',opacity:0}],{duration:800+Math.random()*500,easing:'cubic-bezier(.1,.8,.3,1)',fill:'forwards'}).onfinish=()=>d.remove();
      }
    };
    const run=bl.animate([
      {transform:'translate('+(-W*.62)+'px,'+(H*.98)+'px) scale(1.35) rotate(0deg)',opacity:1,offset:0,easing:'cubic-bezier(.15,.65,.4,1)'},
      {transform:'translate('+(-W*.2)+'px,'+(H*.08)+'px) scale(1) rotate(300deg)',opacity:1,offset:.52,easing:'cubic-bezier(.5,0,.85,.55)'},
      {transform:'translate(0px,'+gy+'px) scale(.66) rotate(620deg)',opacity:1,offset:1}
    ],{duration:1050,fill:'forwards'});
    run.onfinish=()=>{
      gl.classList.add('hit');burst();
      fl.animate([{opacity:.75},{opacity:0}],{duration:420,fill:'forwards'});
      bl.animate([{transform:'translate(0px,'+gy+'px) scale(.66)'},{transform:'translate(0px,'+(gy+H*.07)+'px) scale(.62)'}],{duration:350,easing:'ease-out',fill:'forwards'});
      tx.classList.add('on');
      try{SFX.goal();}catch(e){}
      setTimeout(close,2300);
    };
  }catch(e){toast(created?'Compte créé !':'Connecté !');}
}
function rAcc(){
  const u=CLOUD.user;let h='<div class="cpanel"><h2>COMPTE</h2>';
  if(u){
    h+='<div class="lbme">Connecté : '+esc(u.email||u.displayName||'compte')+(BAN.isStaff()?' • 🛡️ STAFF':'')+'</div>'+
      '<div class="help" id="accSt">'+esc(CLOUD.statusText())+'</div>'+
      '<div class="help">Ta progression (niveau, crédits, objets, rang, réglages) est sauvegardée automatiquement. Connecte-toi avec le même compte sur un autre appareil pour la retrouver.</div>'+
      '<button class="go" data-acc="sync">SAUVEGARDER MAINTENANT</button>'+
      '<button class="go alt" data-acc="logout">SE DÉCONNECTER</button>';
  }else{
    h+='<div class="help">Connecte-toi pour sauvegarder ta progression dans le cloud et la retrouver sur tous tes appareils. Sans compte, elle reste uniquement sur cet appareil.</div>'+
      '<div class="grp"><b>E-MAIL</b><input id="accMail" type="email" autocomplete="email" class="txt" value="'+esc(CLOUD.mail)+'"></div>'+
      '<div class="grp"><b>MOT DE PASSE</b><input id="accPass" type="password" autocomplete="current-password" class="txt"></div>'+
      '<div class="lbmsg" id="accMsg" style="padding:8px 0;text-align:left">'+esc(CLOUD.msg)+'</div>'+
      '<button class="go" data-acc="login">SE CONNECTER</button>'+
      '<button class="go alt" data-acc="signup">CRÉER UN COMPTE</button>'+
      '<button class="go alt" data-acc="google">CONTINUER AVEC GOOGLE</button>'+
      '<div class="grp"><button class="opt" data-acc="reset" style="flex:none;padding:8px 14px">Mot de passe oublié ?</button></div>';
  }
  $scr.innerHTML=h+'</div>';
  const pw=$('accPass');if(pw)pw.onkeydown=e=>{if(e.key==='Enter')CLOUD.act('login');};
}
/* ---------- ÉCRAN DE CONNEXION AU LANCEMENT ----------
   Affiché à l'ouverture du jeu uniquement si personne n'est connecté. La session (token Firebase) est mémorisée
   par le navigateur : une fois connecté, cet écran ne réapparaît plus. « Jouer en invité » le ferme pour cette session. */
const GATE={on:false,
  show(){
    if(GATE.on||CLOUD.user)return;
    if(INTRO.on){INTRO.gate=true;return;}   /* l'intro passe d'abord : l'écran de connexion viendra juste après */
    GATE.on=true;
    if(!$('gateCss')){const st=document.createElement('style');st.id='gateCss';st.textContent=
      '#gate{position:fixed;inset:0;z-index:50;display:flex;align-items:center;justify-content:center;background:radial-gradient(ellipse at 50% 30%,#13265a 0%,#070b1a 70%);overflow:auto;padding:16px;box-sizing:border-box}'+
      '#gate .gb{width:min(92vw,420px);text-align:center}#gate h1{margin:0 0 4px;font-size:clamp(28px,8vw,44px);font-style:italic;letter-spacing:4px}'+
      '#gate .gs{opacity:.75;font-size:14px;margin:0 0 14px}#gate .txt{margin:5px 0}#gate .go{margin-top:9px}'+
      '#gate .gl{background:none;border:0;color:#9db8ff;font:inherit;text-decoration:underline;margin-top:10px;cursor:pointer;font-size:14px}';document.head.appendChild(st);}
    const g=document.createElement('div');g.id='gate';
    g.innerHTML='<div class="gb"><h1>ROCKET ARENA</h1><div class="gs">Connecte-toi pour sauvegarder ta progression dans le cloud et la retrouver sur tous tes appareils.</div>'+
      '<input id="accMail" type="email" autocomplete="email" placeholder="E-mail" class="txt" value="'+esc(CLOUD.mail)+'">'+
      '<input id="accPass" type="password" autocomplete="current-password" placeholder="Mot de passe" class="txt">'+
      '<div class="lbmsg" id="accMsg" style="padding:6px 0;min-height:20px">'+esc(CLOUD.msg)+'</div>'+
      '<button class="go" data-g="login">SE CONNECTER</button>'+
      '<button class="go alt" data-g="signup">CRÉER UN COMPTE</button>'+
      '<button class="go alt" data-g="google">CONTINUER AVEC GOOGLE</button>'+
      '<button class="go alt" data-g="guest">👤 JOUER EN INVITÉ</button>'+
      '<button class="gl" data-g="reset">Mot de passe oublié ?</button></div>';
    document.body.appendChild(g);
    g.querySelectorAll('[data-g]').forEach(b=>b.onclick=()=>{const k=b.dataset.g;if(k==='guest')GATE.hide();else CLOUD.act(k);});
    $('accPass').onkeydown=e=>{if(e.key==='Enter')CLOUD.act('login');};
  },
  hide(){const g=$('gate');if(g)g.remove();GATE.on=false;}
};
if(window.RA_FB||window.RA_FB_ERR)CLOUD.init();else window.addEventListener('ra-fb',()=>CLOUD.init());
/* NEWS : à CHAQUE mise à jour du jeu, on ajoute une entrée EN HAUT de cette liste (la plus récente en premier), avec sa date dans « d » */
const NEWS=[
  {d:'07/11/2026',t:'Pinchs : trajectoires recalculées',b:'Les trajectoires des pinchs ont été recalculées avec les formules du jeu. Kuxir pinch : la démo est maintenant scriptée et le but marche pour un angle de 49° à 65° par rapport au mur (le nez à l\'horizontale). Ceiling pinch : la balle est posée plus près du but (elle rebondissait au-dessus de la barre quand tu plongeais trop à pic) ; la plongée peut aller jusqu\'à 50°.'},
  {d:'07/11/2026',t:'Pack mécaniques : l\'air dribble',b:'Nouvelle mécanique dans le Pack mécaniques : l\'air dribble. La balle rebondit devant toi, tu la décolles puis tu la gardes en l\'air avec des petites touches (3 d\'affilée) avant de marquer. Avec démo du coach.'},
  {d:'07/11/2026',t:'Pack mécaniques : le psycho',b:'Nouvelle mécanique dans le Pack mécaniques : le psycho (du nom du joueur Owen « Psycho » Williams). Tu tapes la balle contre le mur derrière ta propre cage, elle ressort et rentre dans le but adverse. Avec démo du coach.'},
  {d:'07/11/2026',t:'Pack mécaniques : double tap + noms des pinchs',b:'Nouvelle mécanique dans le Pack mécaniques : le double tap (tu frappes la balle sur le tableau arrière, le mur derrière le but, puis tu la reprends en l\'air au rebond pour marquer), avec démo du coach. Les pinchs portent maintenant leurs vrais noms : ground pinch, wall pinch, ceiling pinch et kuxir pinch.'},
  {d:'06/11/2026',t:'Saison 25 : 9 décembre',b:'La prochaine saison (S25) commence le mercredi 9 décembre 2026, comme le vrai Rocket League (date estimée, pas encore confirmée par Psyonix). La saison 24 se termine à cette date.'},
  {d:'06/11/2026',t:'Saisons : calibrage comme le vrai RL',b:'À chaque nouvelle saison, ton MMR est ramené vers la moyenne (reset léger) et ton rang repasse « Non classé » : tu refais tes 10 matchs de placement dans chaque format du Compétitif pour être recalibré, comme dans Rocket League.'},
  {d:'06/11/2026',t:'Bots : les vrais niveaux de RL',b:'Les bots ont maintenant les vrais niveaux de Rocket League : Rookie, Pro et All-Star. Le Rookie roule plus mollement, conduit moins précisément, utilise peu le boost et ne fait presque pas de mécaniques ; le Pro fait des flips et quelques aériens ; l\'All-Star joue à fond : boost, précision et aériens.'},
  {d:'05/11/2026',t:'Flip reset : démo du coach refaite',b:'La démo du coach réussit maintenant à tous les coups : il amène la balle sur le mur, la fait rouler jusqu\'en haut, elle atterrit sur le plafond et retombe lentement, il fait son demi air roll, touche la balle avec les 4 roues (flip reset) et marque dans le but. Ensuite, à toi de le refaire !'},
  {d:'05/11/2026',t:'Arènes en taille réelle',b:'Toutes les arènes sont en taille réelle, y compris l\'entraînement et le Pack mécaniques (plus de mini-arène), et la Dunk House (Hoops) est agrandie : assez de place pour les mécaniques et les aériens.'},
  {d:'05/11/2026',t:'Flip reset : balle au plafond et demi air roll',b:'Le flip reset du Pack mécaniques est refait comme en vrai : tu prends la balle au sol, tu la montes au plafond le long du mur, tu te laisses décoller, tu fais un demi air roll et tu la touches avec les 4 roues (le dessous de la voiture). Ton flip est rendu sans limite de temps, avec un ding, des étincelles et une pastille « FLIP DISPO ». Le coach te montre la démo, et les messages te disent quoi corriger.',go:['lobby','Essayer le flip reset']},
  {d:'25/10/2026',t:'Écran de veille',b:'Si tu ne touches plus à rien dans le menu pendant 1 minute, un écran de veille prend le relais : étoiles qui défilent, titre qui rebondit en changeant de couleur, balle qui rebondit et l\'heure. Touche l\'écran, bouge la souris, appuie sur une touche ou un bouton de manette pour revenir. Le délai se règle dans les réglages.',go:['set','Régler le délai']},
  {d:'25/10/2026',t:'Musique du menu',b:'Le menu a enfin sa musique : 4 morceaux originaux (Neon Garage, Turbo Drift, Stadium Lights, Overtime), composés pour le jeu et joués en direct, avec batterie, basse, nappes et mélodie. Touche le bandeau « Musique » pour passer au morceau suivant. Le volume se règle dans les réglages, et la musique se coupe pendant les matchs.',go:['set','Régler le volume']},
  {d:'25/10/2026',t:'Animation de connexion',b:'Quand tu te connectes ou que tu crées un compte, une balle rentre dans un but, puis « Connecté ! » ou « Compte créé ! » s\'affiche avec une explosion de confettis. Touche l\'écran pour la fermer.'},
  {d:'25/10/2026',t:'Intro du premier lancement',b:'La toute première fois que tu ouvres le jeu, avant même la connexion, une intro se joue : les bots lancent le match, filmé avec des caméras de cinéma, puis l\'un d\'eux fonce sur la balle et marque. Dès que le but est marqué, l\'explosion fait apparaître le nom du jeu en grand, avec ralenti et tremblement. Touche l\'écran, une touche ou un bouton de manette pour la passer. Elle ne se rejoue pas ensuite.'},
  {d:'25/10/2026',t:'Transitions',b:'Les menus n\'apparaissent plus d\'un coup : fondu à l\'ouverture et à la fermeture des fenêtres (pause, fin de match, recherche de partie), éléments de l\'accueil qui glissent un à un, écrans qui montent en douceur, onglets en fondu, rideau noir entre le menu et le match, et boutons plus doux au survol et à l\'appui.'},
  {d:'24/10/2026',t:'Pinch au mur dans le but corrigé',b:'Dans l\'exercice « Pinch au mur dans le but », la voiture retouchait la balle juste après le pinch et annulait son rebond : elle longeait la paroi et finissait à côté de la cage. Maintenant elle rebondit sur le mur, traverse le terrain et rentre dans le but. Le coach réussit la démo.',go:['lobby','Essayer le pinch']},
  {d:'24/10/2026',t:'Pinch au mur dans le but',b:'Nouvel exercice dans le Pack mécaniques : la balle est collée au mur de droite, dans ton camp. Tu fonces dessus, tu la coinces contre la paroi et elle traverse tout le terrain pour rentrer dans le but.',go:['lobby','Essayer le pinch']},
  {d:'24/10/2026',t:'Pinch dans le but plus tolérant',b:'Dans l\'exercice « Pinch du plafond dans le but », le pinch demande moins de vitesse pour être validé et la balle a plus de temps pour rentrer dans la cage.',go:['lobby','Essayer le pinch']},
  {d:'24/10/2026',t:'Pinch du plafond dans le but',b:'Nouvel exercice dans le Pack mécaniques : tu pars du mur, tu montes jusqu\'au plafond, tu redescends sur la balle et tu fais un pinch au sol qui l\'envoie directement dans le but.',go:['lobby','Essayer le pinch']},
  {d:'24/10/2026',t:'Pinch au sol et pinch au mur',b:'Le Pack mécaniques propose maintenant deux exercices séparés : le pinch au sol et le pinch au mur. Le coach te montre chaque version, puis c\'est à toi. Écraser la balle contre le mauvais type de surface ne compte plus.',go:['lobby','Essayer les pinchs']},
  {d:'24/10/2026',t:'Pack mécaniques : le pinch',b:'Nouvelle mécanique dans le Pack mécaniques : le pinch. Le coach te montre comment coincer la balle entre la voiture et le sol, puis c\'est à toi. En match aussi, une balle écrasée contre le sol ou un mur par une voiture repart maintenant beaucoup plus vite.',go:['lobby','Essayer le pinch']},
  {d:'24/10/2026',t:'Dropshot : dalles en deux étapes',b:'Une dalle a maintenant deux étapes : fissurée (motif de fissures visible, la dalle reste pleine) puis détruite (plus rien, c\'est un trou où la balle tombe).'},
  {d:'24/10/2026',t:'Dropshot : arène en taille réelle',b:'L\'Hexa-Arène a maintenant la taille du Stade : même hauteur, même surface, et un plateau de 127 dalles (au lieu de 37). Les voitures et la balle ont donc beaucoup plus de place.'},
  {d:'24/10/2026',t:'Dropshot : arène hexagonale',b:'L\'Hexa-Arène est maintenant un vrai hexagone : murs, rampes, sol, tribunes et plateau de 37 dalles hexagonales. La rangée de dalles du milieu est neutre : les deux équipes peuvent la briser.'},
  {d:'24/10/2026',t:'Dropshot : dalles détruites invisibles, fissures détaillées',b:'Une dalle détruite est maintenant un vrai trou : on ne la voit plus du tout. Une dalle fissurée affiche un motif de fissures bien plus détaillé (fissures en zigzag, ramifications, arcs de contrainte et éclats). La balle y tombe toujours.'},
  {d:'24/10/2026',t:'Dropshot : fissures visibles',b:'Une dalle fissurée reste visible avec un vrai motif de fissures (rayons et ramifications lumineux) au lieu de quelques traits. La balle tombe toujours dans le trou dès que la dalle est fissurée.'},
  {d:'24/10/2026',t:'Cartes privées avec code',b:'Dans le créateur, le bouton « Carte privée » publie ta carte avec un code (ex. K7M2-QX4P) : elle n\'apparaît pas dans la liste publique. Tes amis tapent le code dans Cartes de la communauté pour la jouer. « Mes codes » retrouve les tiens.',go:['comm','Entrer un code']},
  {d:'24/10/2026',t:'Cartes officielles',b:'Les cartes officielles publiées par l\'équipe sont maintenant signées « Rocket Arena » au lieu d\'un nom de joueur, y compris celles déjà publiées. Le nom « Rocket Arena » est réservé.',go:['comm','Voir les cartes']},
  {d:'24/10/2026',t:'Créateur : zoom et déplacement',b:'Dans la vue du dessus du créateur de carte (tous les modes), tu peux zoomer et dézoomer (molette, pincement à deux doigts ou boutons ＋ －) et te balader sur la carte (glisser dans le vide, deux doigts, ou bouton 🖐). ⌂ recentre la vue. En Parkour, la zone où poser des éléments dans le vide est aussi plus grande.',go:['maker','Ouvrir le créateur']},
  {d:'24/10/2026',t:'Parkour : cartes piégées',b:'Dans le créateur de carte en mode Parkour, tu peux maintenant poser des éléments en dehors du sol, dans le vide : faux chemins, plateformes cachées, rampes vers un autre chemin… Idéal pour faire des cartes piégées.',go:['maker','Créer une carte']},
  {d:'24/10/2026',t:'Menu en paysage',b:'L\'accueil et tous les menus plein écran sont réorganisés pour le téléphone en paysage : une seule barre en haut, plus aucun chevauchement, Créateur et Garage mieux rangés.'},
  {d:'24/10/2026',t:'Zoom bloqué',b:'Le double toucher sur SAUT, sur les boutons de jeu ou sur le terrain ne zoome plus la page.'},
  {t:'Nouveau lobby',b:'Cartes Compétitif, Casual, Hors ligne, Match privé et Training. File multiple en compétitif, playlists Arcade qui tournent chaque semaine, Grand groupe 4v4, défis New Driver, hebdo, saison et événement, et groupe avec amis.',go:['lobby','Jouer']},
  {t:'Arènes à l\'échelle réelle',b:'Le Stade et la Patinoire ont désormais les proportions du vrai stade, avec la gravité, les sauts et le boost du vrai jeu.'},
  {t:'Compétitif : 6 playlists',b:'Soccar, Hoops, Rumble, Dropshot, Snow Day et Heatseeker en classé. Chaque playlist a son rang, après 10 matchs de placement.',go:['lobby','Jouer en classé']},
  {t:'Défis du jour',b:'Quatre défis par jour à relever pour gagner des crédits et de l\'XP.',go:['chal','Voir les défis']},
  {t:'Rocket Pass',b:'Monte de palier en jouant et débloque voitures, jantes, explosions et crédits.',go:['pass','Ouvrir le Rocket Pass']}
];
/* Les news les plus anciennes sont effacées toutes seules : on ne garde que les NEWS_MAX dernières (la liste est triée de la plus récente à la plus ancienne). */
const NEWS_MAX=10;if(NEWS.length>NEWS_MAX)NEWS.length=NEWS_MAX;
function openNews(i){
  i=i||0;const n=NEWS[i];
  modal('<div class=\"panel\" style=\"width:min(94vw,760px)\"><h2>NEWS</h2><div class=\"newsw\"><div class=\"newsl\">'+NEWS.map((x,j)=>'<button class=\"nitem'+(j===i?' on':'')+'\" data-n=\"'+j+'\">'+esc(x.t)+'</button>').join('')+'</div><div class=\"newsd\"><h3>'+esc(n.t)+'</h3>'+(n.d?'<small style=\"opacity:.6;letter-spacing:1px\">'+esc(n.d)+'</small>':'')+'<p>'+esc(n.b)+'</p>'+(n.go?'<button class=\"go sm\" id=\"ngo\">'+n.go[1]+'</button>':'')+'</div></div><button class=\"go alt\" id=\"nX\">Fermer</button></div>');
  $('nX').onclick=modalClose;
  document.querySelectorAll('#mbox .nitem').forEach(b=>{b.onclick=()=>openNews(+b.dataset.n);});
  if(n.go)$('ngo').onclick=()=>{modalClose();UI.home=false;if(n.go[0]==='lobby'){SV.menu.mode='ranked';saveSV();}go(n.go[0]);};
}
$('news').onclick=()=>openNews(0);
$('chw').onclick=()=>{UI.home=false;go('chal');};
$('party').onclick=partyPanel;$('friends').onclick=partyPanel;$('leave').onclick=partyLeave;
$('back').onclick=goHome;
/* transition : changer d'onglet fait apparaître le nouveau contenu en fondu */
$scr.addEventListener('click',e=>{if(e.target.closest&&e.target.closest('.tab')){$scr.classList.remove('tabfade');void $scr.offsetWidth;$scr.classList.add('tabfade');clearTimeout($scr._tf);$scr._tf=setTimeout(()=>$scr.classList.remove('tabfade'),400);}});
document.addEventListener('keydown',e=>{if(e.key==='Escape'&&phase==='menu'&&!UI.home&&!$('modal').classList.contains('show')&&!$('queue').classList.contains('show'))goHome();});
function go(scr){
  /* transition : seulement quand on change d'écran (ou qu'on passe accueil ↔ menu), pas à chaque nouveau rendu */
  {const k=scr+'|'+(UI.home?1:0);if(go._k!==k){go._k=k;$scr.classList.remove('anim');void $scr.offsetWidth;$scr.classList.add('anim');clearTimeout(go._t);go._t=setTimeout(()=>$scr.classList.remove('anim'),600);uiEnter();}}
  UI.scr=scr;UI.sel=null;seasonSync();
  const ctx=scr==='lobby'||scr==='tour'||scr==='season'||scr==='pass'||scr==='chal'||scr==='set'||scr==='prof'||scr==='lb'||scr==='acc'||scr==='maker'||scr==='comm'?'lobby':scr;
  lobby.L=SV.load;
  if(ctx==='lobby')lobby.cat='car';
  lobbyEnter(ctx);lobbyRefresh(SV.load);
  if(ctx==='locker')lobbyFocus(UI.cat==='color'?'car':UI.cat);
  if(ctx==='shop')lobbyFocus(UI.cat==='color'?'car':UI.cat);
  ({prof:rProf,lb:rLb,acc:rAcc,lobby:rLobby,tour:rTour,season:rSeason,locker:rLocker,shop:rShop,pass:rPass,chal:rChal,set:rSet,maker:rMaker,comm:rComm})[scr]();
  refreshTop();
}

/* ---------- cartes d'objets ---------- */
function priceOf(it){return UI.scr==='shop'&&UI.feat&&UI.feat[it.id]?Math.round(it.price*.8):it.price;}
function cardHTML(it){
  const r=RAR[it.r],own=owns(it.id),eq=SV.load[it.cat]===it.i&&own;
  let vis;
  if(it.cat==='ban')vis='<div class="cv" style="background:'+it.css+'"><b>'+esc(SV.name)+'</b></div>';
  else if(it.cat==='title')vis='<div class="cv tt">'+esc(it.n)+'</div>';
  else vis='<img class="cv" alt="">';
  const sub=own?(eq?'ÉQUIPÉ':r.n):'💰 '+priceOf(it).toLocaleString('fr-FR');
  return '<button class="card'+(eq?' eq':'')+(own?'':' lock')+(UI.sel===it.id?' sel':'')+'" data-id="'+it.id+'" style="--rc:'+r.c+'">'+vis+'<b>'+esc(it.n)+'</b><small>'+sub+'</small></button>';
}
function hydrate(root){root.querySelectorAll('.card').forEach(b=>{const img=b.querySelector('img');if(img)thumbReq(IT[b.dataset.id],img);});}
function previewItem(it){
  UI.sel=it.id;
  lobbyRefresh(Object.assign({},SV.load,{[it.cat]:it.i}));
  lobbyFocus(it.cat);
  initAudio();
  if(it.cat==='expl')runExplosion(it.ed,SV.menu.team||0,V3(0,6,-22));
  else if(it.cat==='anth')playAnthem(it.i);
  else if(it.cat==='eng'){const a=it.ad;tone(a.base*2,a.base*7,.9,a.t1,.12);tone(a.base*2*a.r2,a.base*7*a.r2,.9,a.t2,.07);}
}
function equipItem(it){
  SV.load[it.cat]=it.i;saveSV();lobby.L=SV.load;lobbyRefresh(SV.load);refreshTop();
}

/* =====================================================================
   LOBBY AVANCÉ : saisons, playlists Arcade, file multiple, New Driver,
   défis hebdo / saison / événement, groupe et amis, mode saison
   ===================================================================== */
const WEEK_MS=6048e5;
function weekNo(){return Math.floor(Date.now()/WEEK_MS);}   /* les semaines commencent le jeudi, comme les resets de RL */
function weekLeftMs(){return WEEK_MS-(Date.now()%WEEK_MS);}
function leftTxt(ms){ms=Math.max(0,ms);const d=Math.floor(ms/864e5),h=Math.floor(ms%864e5/36e5);return d+' j '+h+' h';}
/* débuts de saison : S22, S23 et S24 sont les vraies dates ; S25 est la date annoncée par les sites de suivi (mercredi 9 décembre 2026, ~17 h UTC), pas encore confirmée par Psyonix ; les suivantes sont estimées à ~100 jours */
const SEASONS=[[22,'2026-03-11'],[23,'2026-06-09'],[24,'2026-09-23'],[25,'2026-12-09T17:00:00Z']];
function seasonNow(){
  const now=Date.now();let n=SEASONS[0][0],st=Date.parse(SEASONS[0][1]);
  for(const s of SEASONS){const t=Date.parse(s[1]);if(t<=now){n=s[0];st=t;}}
  if(n===SEASONS[SEASONS.length-1][0]){const L=100*864e5,k=Math.floor((now-st)/L);if(k>0){n+=k;st+=k*L;}}
  let end=st+100*864e5;for(const s of SEASONS){const t=Date.parse(s[1]);if(t>st){end=t;break;}}
  return {n:n,start:st,end:end};
}
/* nouvelle saison, comme dans le vrai RL : reset léger du MMR (on se rapproche de la moyenne) ET le rang repasse « Non classé » :
   il faut refaire les 10 matchs de placement de chaque format (calibrage) ; le MMR après reset sert de point de départ au placement */
function seasonSync(){
  const s=seasonNow();
  if(!SV.season){SV.season=s.n;saveSV();return false;}
  if(SV.season===s.n)return false;
  for(const k in SV.mmr)SV.mmr[k]=Math.round(520+(SV.mmr[k]-520)*.6);
  SV.plc=SV.plc||{};for(const k in SV.plc)SV.plc[k]=0;
  SV.season=s.n;saveSV();toast('Nouvelle saison '+s.n+' : refais tes '+PLACE_N+' matchs de placement pour retrouver ton rang');return true;
}

/* ---------- playlists Arcade (2 emplacements, rotation hebdo, « duel ») ---------- */
const ARC_POOL=['hoops','rumble','drop','snow','heat'];
function arcSync(){
  const wk=weekNo();let a=SV.arc;
  if(!a||typeof a!=='object'||!a.a){a=SV.arc={wk:wk,a:ARC_POOL[wk%5],b:ARC_POOL[(wk+2)%5],pa:0,pb:0};saveSV();}
  else if(a.wk!==wk){
    /* le mode le plus joué de la semaine reste en rotation, l'autre est remplacé */
    const win=a.pa>=a.pb?a.a:a.b,lose=win===a.a?a.b:a.a;
    const rg=mulberry(wk*131+5),c=ARC_POOL.filter(x=>x!==win&&x!==lose);
    a=SV.arc={wk:wk,a:win,b:c[(rg()*c.length)|0],pa:0,pb:0};saveSV();
  }
  [a.a,a.b].forEach((mk,i)=>{
    const base=MODES.find(x=>x.k===mk),e=MODES.find(x=>x.k==='arc'+i);
    e.n='Arcade '+(i?'B':'A')+' : '+base.n;e.ic=base.ic;e.real=mk;
    e.d=base.d+' (rotation hebdo – le plus joué reste, joué '+(i?a.pb:a.pa)+' fois)';
    SZ['arc'+i]=SZ[mk];POPB['arc'+i]=Math.round((POPB[mk]||1000)*.8);
  });
  return a;
}
function arcReal(k){if(k==='arc0'||k==='arc1'){arcSync();return MODES.find(x=>x.k===k).real;}return k;}

/* ---------- file multiple (jusqu'à 6 playlists compétitives) ---------- */
function queueList(){
  const m=SV.menu,l=(m.multi||[]).filter(isRk);
  return (isRk(m.mode)&&l.length>=1)?l:[m.mode];
}
function toggleMulti(k){
  const m=SV.menu;let q=queueList().slice();const i=q.indexOf(k);
  if(i>=0){if(q.length===1){toast('Garde au moins une playlist');return;}q.splice(i,1);}else q.push(k);
  m.multi=q;if(q.indexOf(m.mode)<0)m.mode=q[0];saveSV();rLobby();
}

/* ---------- défis New Driver : obligatoires avant le compétitif ---------- */
const ND_DEFS=[{id:'n:matches',k:'matches',n:'Joue 3 matchs',v:3},{id:'n:goals',k:'goals',n:'Marque 3 buts',v:3},{id:'n:wins',k:'wins',n:'Gagne 1 match',v:1}];
function ndSync(){
  if(!SV.nd){SV.nd=(SV.stats.matches>=5)?{done:true,p:{},auto:true}:{done:false,p:{}};saveSV();}
  return SV.nd;
}
function ndAdd(k,n){
  const nd=ndSync();if(nd.done)return;
  nd.p[k]=(nd.p[k]||0)+n;
  if(ND_DEFS.every(c=>(nd.p[c.k]||0)>=c.v)){
    nd.done=true;
    /* le rang de départ dépend de ta performance pendant les défis */
    const st=380+Math.min(9,nd.p.goals||0)*20+Math.min(3,nd.p.wins||0)*70;
    nd.start=st;SV.ndStart=st;SV.plc=SV.plc||{};
    ['1','2','3'].forEach(key=>{if(!(SV.plc[key]>0))SV.mmr[key]=st;});
  }
}

/* ---------- défis hebdomadaires / de saison / événement ---------- */
function ch2Sync(){
  const wk=weekNo(),sn=seasonNow().n;
  if(!SV.ch2||typeof SV.ch2!=='object')SV.ch2={};
  const c=SV.ch2;
  if(!c.w||c.w.k!==wk)c.w={k:wk,p:{},c:{}};
  if(!c.e||c.e.k!==wk)c.e={k:wk,p:{},c:{}};
  if(!c.s||c.s.k!==sn)c.s={k:sn,p:{},c:{}};
  return c;
}
function ch2Add(k,n){const c=ch2Sync();['w','s','e'].forEach(x=>{c[x].p[k]=(c[x].p[k]||0)+n;});}
const WK_POOL=[
  {k:'goals',n:'Marque {n} buts',v:[20,30],cr:[1200,1800]},
  {k:'wins',n:'Gagne {n} matchs',v:[5,8],cr:[1300,2000]},
  {k:'matches',n:'Joue {n} matchs',v:[10,15],cr:[1000,1500]},
  {k:'assists',n:'Fais {n} passes décisives',v:[8,12],cr:[1200,1800]},
  {k:'saves',n:'Fais {n} arrêts',v:[10,15],cr:[1200,1800]},
  {k:'demos',n:'Fais {n} démolitions',v:[12,20],cr:[1000,1600]}
];
function chWeek(){
  const rg=mulberry(weekNo()*977+13),pool=WK_POOL.slice().sort(()=>rg()-.5).slice(0,3);
  return pool.map((c,i)=>{const t=i===2?1:0;return {id:'w:'+c.k+t,k:c.k,v:c.v[t],cr:c.cr[t],xp:c.cr[t]/2|0,n:c.n.replace('{n}',c.v[t])};});
}
const SN_DEFS=[
  {k:'matches',n:'Joue {n} matchs',v:100,cr:3000},{k:'wins',n:'Gagne {n} matchs',v:50,cr:4000},
  {k:'goals',n:'Marque {n} buts',v:150,cr:3500},{k:'assists',n:'Fais {n} passes décisives',v:60,cr:3000},
  {k:'saves',n:'Fais {n} arrêts',v:80,cr:3000},{k:'demos',n:'Fais {n} démolitions',v:120,cr:3000}
];
function chSeason(){return SN_DEFS.map(c=>({id:'s:'+c.k,k:c.k,v:c.v,cr:c.cr,xp:c.cr/2|0,n:c.n.replace('{n}',c.v)}));}
const EV_DEFS=[
  {t:'Semaine Rumble',k:'m_rumble',n:'Joue {n} matchs Rumble',v:[4,8]},
  {t:'Semaine Hoops',k:'m_hoops',n:'Joue {n} matchs Hoops',v:[4,8]},
  {t:'Semaine Dropshot',k:'m_drop',n:'Joue {n} matchs Dropshot',v:[4,8]},
  {t:'Semaine Snow Day',k:'m_snow',n:'Joue {n} matchs Snow Day',v:[4,8]},
  {t:'Semaine Heatseeker',k:'m_heat',n:'Joue {n} matchs Heatseeker',v:[4,8]}
];
function evNow(){return EV_DEFS[weekNo()%EV_DEFS.length];}
function chEvent(){const e=evNow();return [0,1].map(t=>({id:'e:'+e.k+t,k:e.k,v:e.v[t],cr:[1500,3000][t],xp:[800,1500][t],n:e.n.replace('{n}',e.v[t])}));}
function chClaim(id){
  const sc=id.charAt(0),c2=ch2Sync(),list={w:chWeek,s:chSeason,e:chEvent}[sc];if(!list)return;
  const c=list().find(x=>x.id===id);if(!c)return;const st=c2[sc];
  if(st.c[id]||(st.p[c.k]||0)<c.v)return;
  st.c[id]=1;SV.credits+=c.cr;SV.xp+=c.xp;passSync();saveSV();toast('+'+c.cr+' crédits');refreshTop();rChal();
}
function chRow(c,p,done,claim){
  const pr=Math.min(c.v,p);
  return '<div class="chal"><div><b>'+esc(c.n)+'</b><div class="bar"><i style="width:'+(pr/c.v*100)+'%"></i></div><small>'+pr+' / '+c.v+(c.cr?' • 💰 '+c.cr+' • '+c.xp+' XP':'')+'</small></div>'+
    (claim?(done?'<span class="okk">✔</span>':'<button class="go sm" data-ch="'+c.id+'"'+(pr>=c.v?'':' disabled')+'>Récupérer</button>'):(pr>=c.v?'<span class="okk">✔</span>':''))+'</div>';
}

/* ---------- groupe (party) et liste d'amis ---------- */
const PARTY=[];
const PLAT={epic:'Epic',steam:'Steam',ps:'PlayStation',xbox:'Xbox',sw:'Switch'};
const FRIENDS=(()=>{
  const rg=mulberry(9091),pl=Object.keys(PLAT),nm=['Kaelen','Mira','Taz','Juno','Rex','Sora','Bolt','Lyra','Orion','Zed'];
  return nm.map((n,i)=>({name:n+(i%3===0?'_'+(10+((rg()*89)|0)):''),plat:pl[(rg()*pl.length)|0],mmr:Math.round(300+rg()*700),seed:i,pend:false}));
})();
function friendStat(f){const s=(Math.floor(Date.now()/6e5)+f.seed*7)%5;return s<3?'online':(s===3?'match':'off');}
function partyRank(){
  const m=SV.menu,pk=isRk(m.mode)?m.mode:'ranked',a=[mmrOf(rkKey(pk,modeSize(pk)))].concat(PARTY.map(f=>f.mmr));
  const mx=Math.max.apply(null,a),av=a.reduce((s,x)=>s+x,0)/a.length;
  /* moyenne pondérée : elle penche vers le meilleur joueur à partir de Champion */
  return rankOf(Math.round(mx>=900?mx*.6+av*.4:av));
}
function playSize(k){
  if(k==='exhib')k=SV.menu.exMode||'casual';
  if(k==='workshop')return WS[SV.menu.ws||0].size||2;
  return modeSize(k);
}
function partyOK(list){
  const n=PARTY.length;if(!n)return true;
  for(const k of list){
    const sz=playSize(k);
    if(n+1>sz){toast('Ton groupe ('+(n+1)+' joueurs) est trop grand pour le '+SZN[sz]);return false;}
    if(k==='ranked'&&sz===3&&n===1){
      const my=rankOf(mmrOf(rkKey('ranked',3))).i,fr=rankOf(PARTY[0].mmr).i;
      if(Math.abs(Math.floor(my/3)-Math.floor(fr/3))>3){toast('Écart de rang trop grand en 3v3 Standard (3 rangs maximum)');return false;}
    }
  }
  return true;
}
function partyCfg(size){
  const n=Math.max(0,size-1);
  cfg.party=PARTY.map(f=>f.name).slice(0,n);
  cfg.partySk=PARTY.map(f=>clamp(.35+f.mmr/1100,.35,1)).slice(0,n);
}
function partyUiSync(home){
  const n=PARTY.length;
  $('ui').classList.toggle('inparty',n>0);
  $('party').textContent=n?('👥 GROUPE ('+(n+1)+') • GÉRER'):'⊕ CRÉER UNE ÉQUIPE';
  $('friends').textContent='👥 AMIS • '+FRIENDS.filter(f=>friendStat(f)==='online').length+' EN LIGNE';
  lobby.mates.forEach(g=>{g.visible=!!home&&lobby.ctx==='lobby';});
}
function partyRefresh(){
  if(phase!=='menu')return;
  lobbyMates(lobby.ctx==='lobby'?PARTY.map(f=>f.name):[]);
  lobbyFocus(lobby.cat);
  refreshTop();
  if($('modal').classList.contains('show')&&$('mbox').querySelector('#ppanel'))partyPanel();
  if(UI.scr==='lobby')rOpts();
}
function partyPanel(){
  const online=FRIENDS.filter(f=>friendStat(f)==='online').length,gr=partyRank();
  let h='<div class="panel" id="ppanel" style="width:min(94vw,520px);text-align:left;background:none;border:0;box-shadow:none;padding:0"><h2>GROUPE ET AMIS</h2>';
  h+='<div class="sub">'+(PARTY.length?'Groupe de '+(PARTY.length+1)+' • Rang du groupe : <b style="color:'+gr.c+'">'+esc(gr.n)+'</b>':'Tu es seul. Invite des amis (toutes plateformes) pour jouer ensemble.')+'</div>';
  h+='<h3>TON GROUPE</h3><div class="frow"><div class="finfo"><b>'+esc(SV.name||'Joueur')+'</b> <small>Chef de groupe</small></div></div>';
  h+=PARTY.map(f=>'<div class="frow"><div class="finfo"><b>'+esc(f.name)+'</b> <small>'+PLAT[f.plat]+' • '+esc(rankOf(f.mmr).n)+'</small></div><button class="opt" data-pk="'+esc(f.name)+'" style="flex:none">Exclure</button></div>').join('');
  h+='<h3>AMIS ('+online+' en ligne)</h3><div style="max-height:34vh;overflow:auto">'+FRIENDS.map(f=>{
    const st=friendStat(f),inP=PARTY.indexOf(f)>=0;
    const lab=st==='online'?'<span class="okk">En ligne</span>':'<small>'+(st==='match'?'En match':'Hors ligne')+'</small>';
    return '<div class="frow"><div class="finfo"><b>'+esc(f.name)+'</b> <small>'+PLAT[f.plat]+' • '+esc(rankOf(f.mmr).n)+'</small></div>'+lab+
      (inP?'<span class="okk">✔ Dans le groupe</span>':'<button class="opt" data-inv="'+esc(f.name)+'" style="flex:none"'+((st!=='online'||f.pend||PARTY.length>=3)?' disabled':'')+'>'+(f.pend?'…':'Inviter')+'</button>')+'</div>';
  }).join('')+'</div>';
  h+='<div class="help">Les amis de toutes les plateformes peuvent rejoindre le groupe. En 3v3 Standard à deux, l\'écart de rang maximum est de 3 rangs.</div>';
  h+=(PARTY.length?'<button class="go alt" data-pl="leave">Quitter le groupe</button>':'')+'<button class="go alt" data-a="mclose">Fermer</button></div>';
  modal(h);
}
function partyInvite(name){
  const f=FRIENDS.find(x=>x.name===name);if(!f||f.pend||PARTY.indexOf(f)>=0)return;
  if(PARTY.length>=3){toast('Le groupe est complet');return;}
  if(friendStat(f)!=='online'){toast(f.name+' n\'est pas disponible');return;}
  f.pend=true;toast('Invitation envoyée à '+f.name);partyRefresh();
  setTimeout(()=>{
    f.pend=false;
    if(rnd()<.9&&PARTY.length<3){PARTY.push(f);toast(f.name+' a rejoint le groupe');}
    else toast(f.name+' a refusé l\'invitation');
    partyRefresh();
  },1100+rnd()*1200);
}
function partyLeave(){
  modal('<div class="panel small"><h2>QUITTER LE GROUPE ?</h2><button class="go" data-pl="leaveY">Quitter</button><button class="go alt" data-a="mclose">Annuler</button></div>');
}
$('modal').addEventListener('click',e=>{
  const t=e.target.closest('[data-inv],[data-pk],[data-pl]');if(!t)return;
  const d=t.dataset;
  if(d.inv)partyInvite(d.inv);
  else if(d.pk){const i=PARTY.findIndex(f=>f.name===d.pk);if(i>=0){toast(PARTY[i].name+' a quitté le groupe');PARTY.splice(i,1);partyRefresh();}}
  else if(d.pl==='leave')partyLeave();
  else if(d.pl==='leaveY'){PARTY.length=0;modalClose();toast('Tu as quitté le groupe');partyRefresh();}
});

/* ---------- cartes Workshop (mutateurs prédéfinis) ---------- */
const WS=[
  {n:'Lune',d:'Gravité basse et grosse balle : tout flotte.',mut:{grav:.5,ball:1.5},size:2},
  {n:'Balle géante',d:'Une balle 2,4 fois plus grande à pousser dans les buts.',mut:{ball:2.4},size:3},
  {n:'Flipper',d:'Balle petite, rapide et très rebondissante.',mut:{bounce:1.25,bspeed:1.3,ball:.6},size:2},
  {n:'Hoops turbo',d:'Le basket en voiture avec du boost illimité.',mut:{arena:'hoops',boost:'inf'},size:2},
  {n:'Dropshot lourd',d:'Dropshot avec une gravité haute.',mut:{arena:'drop',grav:1.5},size:3},
  {n:'Rumble sur glace',d:'Power-ups sur la patinoire, avec un palet.',mut:{arena:'snow',rumble:1},size:3},
  {n:'Course folle',d:'Jeu rapide et boost illimité.',mut:{speed:1.4,boost:'inf'},size:2}
];

/* ---------- mode saison (championnat hors ligne) ---------- */
const LG_N=7,LG_PRIZE=[2500,1600,1100,800,600,400,300,200];
function lgNew(){
  const rg=mulberry((Date.now()/1000)|0),pool=TOUR_NAMES.slice().sort(()=>rg()-.5);
  SV.lg={season:seasonNow().n,i:0,pts:0,w:0,l:0,done:false,teams:[(SV.name||'Joueur')+' & co'].concat(pool.slice(0,7)),tp:[0,0,0,0,0,0,0,0]};saveSV();
}
function lgFinish(win){
  const g=SV.lg;if(!g||g.done)return;
  const rg=mulberry((Date.now()/100)|0);
  if(win){g.pts+=3;g.w++;}else g.l++;
  for(let j=1;j<8;j++){if(j===g.i+1){if(!win)g.tp[j]+=3;}else if(rg()<.5)g.tp[j]+=3;}
  g.i++;
  if(g.i>=LG_N){g.done=true;g.pos=1+g.tp.slice(1).filter(p=>p>g.pts).length;g.prize=LG_PRIZE[g.pos-1];SV.credits+=g.prize;}
  saveSV();
}
function lgSummary(win){
  const g=SV.lg;if(!g)return '';
  if(g.done)return '🏅 Saison terminée : '+g.pos+(g.pos===1?'re':'e')+' place • +'+g.prize+' 💰';
  return (win?'Victoire':'Défaite')+' • '+g.pts+' pts • prochain match dans le Mode saison';
}
function rSeason(){
  if(!SV.lg)lgNew();const g=SV.lg;
  const order=g.teams.map((n,i)=>[i,i===0?g.pts:g.tp[i]]).sort((a,b)=>b[1]-a[1]||(a[0]===0?-1:(b[0]===0?1:0)));
  let h='<div class="cpanel wide"><h2>MODE SAISON</h2><div class="sub">Saison '+seasonNow().n+' • '+(g.done?'terminée':'Match '+(g.i+1)+' / '+LG_N+' • adversaire : '+esc(g.teams[g.i+1]))+'</div>';
  h+='<table><tr><th>#</th><th>ÉQUIPE</th><th>PTS</th></tr>'+order.map((o,p)=>'<tr'+(o[0]===0?' style="background:rgba(61,139,255,.22)"':'')+'><td>'+(p+1)+'</td><td>'+esc(g.teams[o[0]])+'</td><td>'+o[1]+'</td></tr>').join('')+'</table>';
  h+=(g.done?'<div class="sub" style="margin-top:8px">'+lgSummary()+'</div><button class="go" data-a="lgNew">NOUVELLE SAISON</button>':'<button class="go" data-a="lgPlay">JOUER LE MATCH ▶</button>')+'<button class="go alt" data-go="lobby">Retour</button></div>';
  $scr.innerHTML=h;
}

/* ---------- LOBBY (jouer) ---------- */
/* cartes du menu Jouer, comme le vrai RL : Casual / Compétitif / Tournois / Entraînement / Match privé */
const CARDS=[
  {k:'ranked',n:'Compétitif',ic:'🏆',m:['ranked','rk_hoops','rk_rumble','rk_drop','rk_snow','rk_heat']},
  {k:'casual',n:'Casual',ic:'⚽',m:['casual','arc0','arc1','big']},
  {k:'offline',n:'Hors ligne',ic:'📴',m:['exhib','season','workshop','tour']},
  {k:'comm',n:'Communauté',ic:'🌍',m:['comm']},
  {k:'custom',n:'Match privé',ic:'🔧',m:['custom']},
  {k:'train',n:'Training',ic:'🎓',m:['free','train','moves']}
];
const POPB={big:9800,casual:41200,ranked:18350,hoops:3120,rumble:5400,drop:1900,snow:2650,heat:2100,tour:860,rk_hoops:2210,rk_rumble:3050,rk_drop:1260,rk_snow:1740,rk_heat:1380};
function popHTML(k){
  const b=POPB[k];if(!b)return '';
  const h=Math.floor(Date.now()/36e5),v=Math.round(b*(1+.09*Math.sin(h*1.7+k.length*2.3)));
  return '<em class=\"popb\">👥 '+v.toLocaleString('fr-FR')+'</em>';
}
function cardOf(mode){return CARDS.find(c=>c.m.indexOf(mode)>=0)||CARDS[0];}
function rLobby(){
  arcSync();
  if(!CARDS.some(c=>c.m.indexOf(SV.menu.mode)>=0)){SV.menu.mode='casual';saveSV();}
  const m=SV.menu,card=cardOf(m.mode),sn=seasonNow();
  $scr.innerHTML='<div class="rpanel"><div class="phead"><h2>JOUER</h2><div class="sub">Saison '+sn.n+' • fin dans '+leftTxt(sn.end-Date.now())+' • choisis un mode puis lance la recherche.</div></div>'+
    '<div class="pbody"><div class="cards">'+CARDS.map(c=>'<button class="rcard'+(c===card?' on':'')+'" data-card="'+c.k+'"><i>'+c.ic+'</i><b>'+c.n+'</b></button>').join('')+'</div>'+(card.m.length>1?'<div class="modes">':'<div class="modes hide">')+MODES.filter(x=>card.m.indexOf(x.k)>=0).map(x=>'<button class="mode'+(m.mode===x.k?' on':'')+'" data-mode="'+x.k+'"><i>'+x.ic+'</i><b>'+x.n+'</b><small>'+x.d+'</small>'+popHTML(x.k)+'</button>').join('')+'</div><div id="mopts"></div></div>'+
    '<div class="pfoot"><button class="go" data-a="play">JOUER ▶</button></div></div>'+
    '<div id="lobinfo"></div>';
  rOpts();
}
function grp(title,key,vals,cur,isMut){
  return '<div class="grp"><b>'+title+'</b><div class="opts">'+vals.map(v=>'<button class="opt'+(String(cur)===String(v[0])?' on':'')+'" '+(isMut?'data-mut="':'data-opt="')+key+'" data-v="'+v[0]+'">'+v[1]+'</button>').join('')+'</div></div>';
}
const NOOPT=['free','train','moves','goalrun','tour','season'];
function rOpts(){
  const m=SV.menu,k=m.mode,box=$('mopts');if(!box)return;
  arcSync();
  let h='';
  const mode=MODES.find(x=>x.k===k),fk=k==='exhib'?(m.exMode||'casual'):k;
  h+='<div class="sub" style="margin-top:10px"><b>'+mode.ic+' '+mode.n+'</b> — '+mode.d+'</div>';
  if(k==='exhib')h+=grp('PLAYLIST','exMode',[['casual','Soccar'],['hoops','Hoops'],['rumble','Rumble'],['drop','Dropshot'],['snow','Snow Day'],['heat','Heatseeker'],['big','Grand groupe']],fk);
  if(k==='workshop')h+=grp('CARTE WORKSHOP','ws',WS.map((w,i)=>[i,w.n]),m.ws||0)+'<div class="help">'+esc(WS[m.ws||0].d)+' ('+SZN[WS[m.ws||0].size||2]+')</div>';
  if(SZ[fk]&&k!=='season'&&k!=='workshop')h+=grp('FORMAT','size',SZ[fk].map(s=>[s,SZN[s]]),modeSize(fk));
  if(isRk(k)){
    const kk=rkKey(k,modeSize(k)),mm=mmrOf(kk),rk=rankInfo(kk),q=queueList(),nd=ndSync();
    h+='<div class="rank" style="border-color:'+rk.c+'"><span style="color:'+rk.c+'">'+rk.n.toUpperCase()+'</span><small>'+(rk.un?'Matchs de placement : '+(PLACE_N-rk.left)+' / '+PLACE_N:mm+' MMR')+' • matchs de 5 min, bots adaptés à ton niveau</small></div>';
    if(!nd.done)h+='<div class="help" style="color:#ffd166">🔒 Compétitif verrouillé : termine les défis New Driver ('+ND_DEFS.filter(c=>(nd.p[c.k]||0)>=c.v).length+' / '+ND_DEFS.length+') dans Défis.</div>';
    h+='<div class="grp"><b>FILE D\'ATTENTE MULTIPLE ('+q.length+' / 6)</b><div class="opts">'+MODES.filter(x=>isRk(x.k)).map(x=>'<button class="opt'+(q.indexOf(x.k)>=0?' on':'')+'" data-multi="'+x.k+'">'+x.n.replace(' classé','')+' '+modeSize(x.k)+'v'+modeSize(x.k)+'</button>').join('')+'</div></div>';
  }
  if(!isRk(k)&&NOOPT.indexOf(k)<0)h+=grp('NIVEAU DES BOTS','diff',[[0,'Rookie'],[1,'Pro'],[2,'All-Star']],m.diff);
  if(!isRk(k)&&NOOPT.indexOf(k)<0)h+=grp('DURÉE','dur',[[2,'2 min'],[3,'3 min'],[5,'5 min'],[0,'Illimitée']],m.dur);
  if(k==='train')h+=grp('PACK','pack',PACKS.map((p,i)=>[i,p.n]),m.pack||0)+'<div class="help">'+PACKS[m.pack||0].d+'</div>';
  if(k==='moves'){
    const cur=(m.mv>=0&&m.mv<MOVES.length)?m.mv:0;m.mv=cur;const mv=MOVES[cur],pg=(SV.mv&&SV.mv[mv.id])||{ok:0,att:0};
    h+=grp('MÉCANIQUE','mv',MOVES.map((x,i)=>[i,x.ic+' '+x.n]),cur);
    h+=grp('DÉMO DU COACH AU DÉBUT','mvDemo',[[1,'Oui'],[0,'Non']],m.mvDemo===undefined?1:m.mvDemo);
    h+='<div class="mvcard"><b>'+mv.ic+' '+esc(mv.n)+' <em>'+mv.lvl+'</em></b><p>'+esc(mv.def)+'</p><ol>'+mv.steps.map(x=>'<li>'+x+'</li>').join('')+'</ol><div class="mvg">🎯 '+esc(mv.goal)+'</div><small>Tes réussites : '+pg.ok+' / '+pg.att+'</small></div>';
    h+='<div class="help">Pendant l\'entraînement : V = revoir la démo • R = recommencer • N / B = move suivant / précédent • Entrée = passer la démo.</div>';
  }
  if(k!=='free'&&k!=='train'&&k!=='moves'&&k!=='goalrun')h+=grp('ÉQUIPE','team',[[0,'Bleue'],[1,'Orange']],m.team);
  if(k==='custom')CUSTOM.forEach(c=>{h+=grp(c[1],c[0],c[2],(m.mut||{})[c[0]]!==undefined?(m.mut||{})[c[0]]:c[2][c[0]==='grav'||c[0]==='ball'||c[0]==='bounce'||c[0]==='bspeed'||c[0]==='speed'?1:0][0],true);});
  if(k==='tour'){const t=SV.tour;h+='<div class="help">'+(t&&t.alive&&!t.champ?'Tournoi en cours : '+TOUR_ROUNDS[t.round]+'.':'Nouveau tournoi : 3 tours à élimination directe.')+'</div>';}
  if(k==='season'){const g=SV.lg;h+='<div class="help">'+(g&&!g.done?'Saison en cours : match '+(g.i+1)+' / '+LG_N+' • '+g.pts+' pts.':'Nouveau championnat : 7 matchs, 3 points par victoire.')+'</div>';}
  if(PARTY.length&&k!=='free'&&k!=='train'&&k!=='moves'&&k!=='goalrun'){const gr=partyRank();h+='<div class="help">👥 Groupe de '+(PARTY.length+1)+' • rang du groupe : <b style="color:'+gr.c+'">'+esc(gr.n)+'</b> • '+esc(PARTY.map(f=>f.name).join(', '))+'</div>';}
  h+=grp('CAMÉRA','ballCam',[[1,'Focus balle'],[0,'Libre']],SV.set.ballCam);
  box.innerHTML=h;
  $('lobinfo').innerHTML='<b>'+mode.ic+' '+mode.n+'</b><small>'+((SZ[fk]&&k!=='season'&&k!=='workshop')?SZN[modeSize(fk)]:'')+'</small>';
}
/* ---------- recherche de match ---------- */
const FAKEOPP=['Nova','Blaze','Vortex','Pixel','Turbo','Zephyr','Comet','Rogue','Drift','Apex','Echo','Flux'];
function setCfg(k0){
  const m=SV.menu;let k=k0;
  if(k==='exhib')k=m.exMode||'casual';
  k=arcReal(k);
  cfg.mode=k;cfg.mut=null;cfg.lg=false;
  cfg.size=modeSize(k);cfg.diff=m.diff;cfg.dur=isRk(k)?5:m.dur;cfg.team=m.team;cfg.pack=m.pack||0;cfg.mv=(m.mv>=0&&m.mv<MOVES.length)?m.mv:0;cfg.mvDemo=m.mvDemo===undefined?1:m.mvDemo;cfg.skill=0;
  if(k0==='workshop'){const w=WS[m.ws||0];cfg.mode='custom';cfg.mut=Object.assign({},w.mut);cfg.size=w.size||2;}
  if(isRk(k)){const mm=mmrOf(rkKey(k,cfg.size));cfg.skill=clamp(.32+mm/1000,.32,1.02);}
  if(k==='free'||k==='train'||k==='moves'||k==='goalrun'){cfg.party=[];cfg.partySk=[];}else partyCfg(cfg.size);
}
function startQueue(cb,list){
  const q=$('queue'),box=$('qbox');q.classList.add('show');
  initAudio();
  let t=0;
  const nm=k=>{const x=MODES.find(y=>y.k===k);return x?x.n.replace(' classé','')+(SZ[k]?' '+modeSize(k)+'v'+modeSize(k):''):'';};
  let wait=10+((Math.max(0,MODES.findIndex(x=>x.k===SV.menu.mode))*7)%20);
  if(list&&list.length>1)wait=Math.max(5,wait-(list.length-1)*2);
  box.innerHTML='<h2>RECHERCHE DE MATCH…</h2><div class=\"spin\"></div><div class=\"sub\" id=\"qt\">0:00</div>'+(list?'<div class="sub">'+list.map(nm).join(' • ')+'</div>':'')+(PARTY.length?'<div class="sub">👥 Groupe de '+(PARTY.length+1)+'</div>':'')+'<div class=\"sub\">Temps d\'attente moyen : 0:'+wait+'</div><div class=\"qmenu\"><button class=\"go alt\" id=\"qf\">🛠️ FREEPLAY</button><button class=\"go alt\" id=\"qch\">🎯 DÉFIS</button><button class=\"go alt\" id=\"qtr\">🎓 TRAINING</button></div><button class=\"go alt\" id=\"qc\">Annuler</button>';
  let cancelled=false;
  const stopQ=()=>{cancelled=true;q.classList.remove('show');clearInterval(iv);clearTimeout(tm);};
  $('qc').onclick=stopQ;
  $('qf').onclick=()=>{stopQ();SV.menu.mode='free';saveSV();doPlay();};
  $('qch').onclick=()=>{stopQ();UI.home=false;go('chal');};
  $('qtr').onclick=()=>{stopQ();SV.menu.mode='train';saveSV();UI.home=false;go('lobby');};
  const iv=setInterval(()=>{t++;const e=$('qt');if(e)e.textContent=Math.floor(t/60)+':'+String(t%60).padStart(2,'0');},1000);
  const tm=setTimeout(()=>{
    clearInterval(iv);if(cancelled)return;
    SFX.beep(true);
    const pick=list?list[(rnd()*list.length)|0]:SV.menu.mode;
    if(list){
      setCfg(pick);
      if(pick==='arc0'||pick==='arc1'){const a=SV.arc;if(pick==='arc0')a.pa++;else a.pb++;saveSV();}
    }
    const pm=MODES.find(x=>x.k===pick)||MODES[0],psz=SZ[pick]?modeSize(pick):0;
    const rg=mulberry((Date.now()/1000)|0),opp=[];for(let i=0;i<Math.max(1,psz||1);i++)opp.push(FAKEOPP[(rg()*FAKEOPP.length)|0]);
    box.innerHTML='<h2 style="color:#7dffb0">MATCH TROUVÉ !</h2><div class="sub">'+pm.n.replace(' classé','')+(isRk(pick)?' (classé)':'')+' • '+(psz?SZN[psz]:'')+'</div><div class="vs"><b class="tb2">'+esc(SV.name)+PARTY.slice(0,Math.max(0,(psz||1)-1)).map(f=>' · '+esc(f.name)).join('')+'</b><span>VS</span><b class="to2">'+opp.join(' · ')+'</b></div>';
    setTimeout(()=>{if(cancelled)return;q.classList.remove('show');cb();},1500);
  },2200+rnd()*1200);
}
function doPlay(){
  const m=SV.menu,k=m.mode;
  if(k==='season'){go('season');return;}
  if(k==='comm'){go('comm');return;}
  if(k==='free'||k==='train'||k==='moves'||k==='goalrun'){setCfg(k);cfg.size=1;cfg.party=[];cfg.partySk=[];startMatch();return;}
  if(k==='tour'){setCfg(k);if(!SV.tour||!SV.tour.alive||SV.tour.champ)tourNew();go('tour');return;}
  const list=queueList(),direct=(k==='exhib'||k==='workshop');
  if(list.some(isRk)&&!ndSync().done){toast("Termine d'abord les défis New Driver pour jouer en compétitif");UI.home=false;UI.ctab='n';go('chal');return;}
  if(!partyOK(direct?[k]:list))return;
  if(direct){setCfg(k);startMatch();return;}
  startQueue(()=>startMatch(),list);
}
/* =====================================================================
   CRÉATEUR DE CARTE : obstacles solides (physique SDF) + pastilles de boost
   ===================================================================== */
const CMT={ /* type : nom, icône, forme, dimensions par défaut [w,h,d], altitude, inclinaison, dims éditables */
  bloc:{n:'Bloc',ic:'⬛',s:'box',v:[8,8,8],e:0,p:0,ed:'whd'},
  mur:{n:'Mur',ic:'▬',s:'box',v:[24,10,2.5],e:0,p:0,ed:'whd'},
  cube:{n:'Cube géant',ic:'🧊',s:'box',v:[18,18,18],e:0,p:0,ed:'whd'},
  rampe:{n:'Rampe',ic:'◢',s:'box',v:[14,1.4,26],e:0,p:-22,ed:'whdp'},
  rampe2:{n:'Rampe raide',ic:'◥',s:'box',v:[14,1.4,20],e:0,p:-38,ed:'whdp'},
  dalle:{n:'Plateforme',ic:'▭',s:'box',v:[20,1.6,20],e:10,p:0,ed:'whd'},
  pont:{n:'Pont',ic:'🌉',s:'box',v:[8,1.6,40],e:12,p:0,ed:'whd'},
  pilier:{n:'Pilier',ic:'🏛️',s:'cyl',v:[4,22,4],e:0,p:0,ed:'wh'},
  plot:{n:'Plot bas',ic:'🛑',s:'cyl',v:[6,3,6],e:0,p:0,ed:'wh'},
  dome:{n:'Dôme',ic:'🔵',s:'sph',v:[14,14,14],e:-4,p:0,ed:'w'},
  boule:{n:'Boule',ic:'⚫',s:'sph',v:[6,6,6],e:3,p:0,ed:'w'},
  anneau:{n:'Anneau',ic:'⭕',s:'tor',v:[18,18,2],e:3,p:90,ed:'wdp'},
  cerceau:{n:'Cerceau couché',ic:'🍩',s:'tor',v:[14,14,2],e:6,p:0,ed:'wdp'},
  but:{n:'But',ic:'🥅',s:'goal',v:[42,15,21],e:0,p:0,ed:'whd'},
  arrivee:{n:'Arrivée',ic:'🏁',s:'fin',v:[24,12,4],e:0,p:0,ed:'whd'},
  pad:{n:'Boost petit',ic:'🟠',s:'pad',v:[3.6,0,3.6],e:0,p:0,ed:''},
  bpad:{n:'Boost gros',ic:'🔶',s:'pad',v:[7,0,7],e:0,p:0,ed:''}
};
const CMCOL=['#5aa9ff','#ff8a3d','#35e0a1','#ffd23f','#c27bff','#ff5d7a','#cfd8e8'];
function cmDims(){return ED.map.small?[40,60]:[96,120];} /* taille du sol (éditeur) */
/* PARKOUR : on peut poser des éléments hors du sol, dans le vide (cartes piégées : faux chemins, plateformes cachées…) */
const CMEXT=160,CMLIM=130,CMLIM_PK=300; /* marge de pose hors du sol / limite des données (normal / parkour) */
function cmExt(m){return m&&m.mode==='parkour'?CMEXT:0;}
const GSC=[[.6,'Petites'],[1,'Normales'],[1.4,'Grandes'],[1.8,'Géantes']]; /* taille des cages classiques */
const GPRE=[['Mini',12,7,9],['Petit',24,10,14],['Normal',42,15,21],['Grand',60,22,26],['Géant',80,32,32]]; /* tailles prédéfinies d'un but */
const GSH=[['rect','Rectangle'],['circle','Cercle'],['hex','Hexagone'],['tri','Triangle']]; /* « masques » : forme des buts et des zones */
const SHN={rect:0,circle:24,hex:6,tri:3,ring:0},SHMINW={rect:8,circle:14,hex:16,tri:28,ring:14};
const GRP=[['Mini',14],['Petit',20],['Normal',28],['Grand',40],['Géant',60]]; /* diamètres prédéfinis d'un but 2D (cerceau vertical) */
function shOf(o){return SHN[o.sh]!==undefined?o.sh:'rect';}
/* polygone régulier : le côté k a sa normale à k·2π/N (0 = avant, +z local) ; le sommet k est entre les côtés k et k+1 */
function polyV(N,Rc){const v=[];for(let k=0;k<N;k++){const a=(k+.5)*2*Math.PI/N;v.push([Math.sin(a)*Rc,Math.cos(a)*Rc]);}return v;}
function polyOpen(sh,k,N){if(sh==='circle'){let r=k*2*Math.PI/N;r=Math.atan2(Math.sin(r),Math.cos(r));return Math.abs(r)<38*Math.PI/180;}return k===0;}
function polyIn(g,lx,lz,m){if(g.sh==='circle')return Math.hypot(lx,lz)<g.Rc-m;
  for(let k=0;k<g.N;k++){const a=k*2*Math.PI/g.N;if(lx*Math.sin(a)+lz*Math.cos(a)>g.ap-m)return false;}return true;}
const GTEAM=['Bleu','Orange','Neutre'],GCOL=['#2f8bff','#ff8a2a','#35e0a1'];
function cmTexRows(label,key,url){return '<div class="row"><span>'+label+'</span>'+(url?'<img src="'+url+'" style="width:30px;height:30px;object-fit:cover;border:1px solid #fff">':'')+'<button class="t" data-tx="'+key+'" style="width:auto;padding:6px 10px">🖼 '+(url?'Changer':'Image')+'</button>'+(url?'<button class="t" data-txx="'+key+'" style="width:34px">✕</button>':'')+'</div>';}
function cmGoalCol(o,mode){return mode==='solo'?o.c:GCOL[o.team|0];}
function cmAlone(m){return m.mode==='solo'||m.mode==='parkour';} /* un seul joueur */
function cmVExt(o){const t=CMT[o.t],p=o.pitch*Math.PI/180,s=Math.abs(Math.sin(p)),c=Math.abs(Math.cos(p));
  if(t.s==='box')return o.d/2*s+o.h/2*c;if(t.s==='cyl')return o.h/2;if(t.s==='sph')return o.w/2;if(t.s==='tor')return o.w/2*s+o.d/2;return 0;}
/* compile : pré-calcule rotations et rayon englobant (appelé au lancement d'un match) */
function cmCompile(m){
  const obs=[],pads=[],goals=[],fins=[];
  const mode=m.mode==='solo'?'solo':(m.mode==='parkour'?'parkour':'bots');
  const gx=(o,g)=>{g.n=o.n;g.bd=o.bd;g.by=o.by;g.hv=o.hv;return g;}; /* séquence : paramètres de chaque but */
  m.obj.slice(0,160).forEach(o0=>{const o=cmClean(o0,mode==='parkour'?CMLIM_PK:CMLIM);if(!o)return;const t=CMT[o.t];
    if(t.s==='pad'){pads.push({x:o.x,z:o.z,big:o.t==='bpad'});return;}
    if(t.s==='fin'){ /* arrivée de parkour : zone traversable (pas solide) */
      if(mode==='parkour'){const yw=o.yaw*Math.PI/180,sh=o.sh||'rect',N=SHN[sh],Rc=o.w/2;fins.push({x:o.x,z:o.z,y0:o.e,w:o.w,h:o.h,d:o.d,cy:Math.cos(yw),sy:Math.sin(yw),yaw:yw,sh:sh,N:N,Rc:Rc,ap:N?Rc*Math.cos(Math.PI/N):0});}
      return;}
    if(t.s==='goal'){if(mode==='parkour')return; /* but : coque (fond, flancs, toit, sol) = boîtes solides invisibles ; l'avant (+z local) reste ouvert */
      const yw=o.yaw*Math.PI/180,c=Math.cos(yw),s=Math.sin(yw),T=.8,hw=o.w/2,hd=o.d/2;
      if(o.sh==='ring'){ /* but 2D : cerceau vertical en hauteur. L'anneau est solide, la balle doit passer DANS le trou (axe = avant du but) */
        const Rc=hw,rt=Math.min(1.2,Math.max(.6,Rc*.05));
        obs.push({t:'but',s:'tor',hide:true,x:o.x,y:o.e+Rc,z:o.z,cy:c,sy:s,cp:0,sp:1,yaw:yw,pitch:Math.PI/2,col:o.c,hx:Rc,hy:rt,hz:rt,r:Rc,Rc:Rc,rt:rt,w:Rc*2,h:rt*2,d:rt*2,br:Rc+rt});
        goals.push(gx(o,{x:o.x,z:o.z,y0:o.e,w:o.w,h:o.w,d:rt*2,cy:c,sy:s,yaw:yw,team:o.team|0,col:o.c,mx:o.x,mz:o.z,fx:s,fz:c,sh:'ring',N:0,Rc:Rc,rt:rt,ap:0}));
        return;}
      if(o.sh&&o.sh!=='rect'){ /* but rond / hexagonal / triangulaire : un mur par côté (sauf l'ouverture), toit et sol en disque */
        const sh=o.sh,N=SHN[sh],Rc=hw,ap=Rc*Math.cos(Math.PI/N),step=2*Math.PI/N,el=Rc*Math.sin(Math.PI/N);
        const add=(q)=>{q.t='but';q.hide=true;q.col=o.c;obs.push(q);};
        for(let k=0;k<N;k++){if(polyOpen(sh,k,N))continue;const psi=yw+k*step,nx=Math.sin(psi),nz=Math.cos(psi),hy=(o.h+T)/2,hx=el+T,hz=T/2;
          add({s:'box',x:o.x+nx*(ap+T/2),y:o.e+hy,z:o.z+nz*(ap+T/2),cy:Math.cos(psi),sy:Math.sin(psi),cp:1,sp:0,yaw:psi,pitch:0,hx:hx,hy:hy,hz:hz,r:hx,Rc:hx,rt:hz,w:hx*2,h:hy*2,d:hz*2,br:Math.hypot(hx,hy,hz)});}
        const rr=(Rc+ap)/2+T;
        add({s:'cyl',x:o.x,y:o.e+o.h+T/2,z:o.z,cy:1,sy:0,cp:1,sp:0,yaw:0,pitch:0,hx:rr,hy:T/2,hz:rr,r:rr,Rc:rr,rt:T,w:rr*2,h:T,d:rr*2,br:Math.hypot(rr,T/2)});
        if(o.e>.5)add({s:'cyl',x:o.x,y:o.e-T/2,z:o.z,cy:1,sy:0,cp:1,sp:0,yaw:0,pitch:0,hx:rr,hy:T/2,hz:rr,r:rr,Rc:rr,rt:T,w:rr*2,h:T,d:rr*2,br:Math.hypot(rr,T/2)});
        goals.push(gx(o,{x:o.x,z:o.z,y0:o.e,w:o.w,h:o.h,d:o.w,cy:c,sy:s,yaw:yw,team:o.team|0,col:o.c,mx:o.x+s*ap,mz:o.z+c*ap,fx:s,fz:c,sh:sh,N:N,Rc:Rc,ap:ap}));
        return;}
      const W=(lx,ly,lz)=>({x:o.x+c*lx+s*lz,y:o.e+ly,z:o.z-s*lx+c*lz});
      const box=(lx,ly,lz,hx,hy,hz)=>{const p=W(lx,ly,lz);obs.push({t:'but',s:'box',hide:true,x:p.x,y:p.y,z:p.z,cy:c,sy:s,cp:1,sp:0,yaw:yw,pitch:0,col:o.c,hx:hx,hy:hy,hz:hz,r:hx,Rc:hx,rt:hz,w:hx*2,h:hy*2,d:hz*2,br:Math.hypot(hx,hy,hz)});};
      box(0,(o.h+T)/2,-hd-T/2,hw+T,(o.h+T)/2,T/2);                 /* fond */
      box(-hw-T/2,(o.h+T)/2,-T/2,T/2,(o.h+T)/2,hd+T/2);            /* flanc gauche */
      box(hw+T/2,(o.h+T)/2,-T/2,T/2,(o.h+T)/2,hd+T/2);             /* flanc droit */
      box(0,o.h+T/2,-T/2,hw+T,T/2,hd+T/2);                         /* toit */
      if(o.e>.5)box(0,-T/2,-T/2,hw+T,T/2,hd+T/2);                  /* sol (but surélevé) */
      const mx=o.x+s*hd,mz=o.z+c*hd;
      goals.push(gx(o,{x:o.x,z:o.z,y0:o.e,w:o.w,h:o.h,d:o.d,cy:c,sy:s,yaw:yw,team:o.team|0,col:o.c,mx:mx,mz:mz,fx:s,fz:c,sh:'rect',N:0}));
      return;}
    const y=o.e+cmVExt(o),yw=o.yaw*Math.PI/180,pi=o.pitch*Math.PI/180;
    const q={t:o.t,s:t.s,x:o.x,y:y,z:o.z,cy:Math.cos(yw),sy:Math.sin(yw),cp:Math.cos(pi),sp:Math.sin(pi),yaw:yw,pitch:pi,col:o.c,
      hx:o.w/2,hy:o.h/2,hz:o.d/2,r:o.w/2,Rc:o.w/2,rt:o.d/2,w:o.w,h:o.h,d:o.d};
    q.br=t.s==='box'?Math.hypot(q.hx,q.hy,q.hz):t.s==='cyl'?Math.hypot(q.r,q.hy):t.s==='sph'?q.r:q.Rc+q.rt;
    obs.push(q);});
  const gs=GSC.some(q=>q[0]===+m.gs)?+m.gs:1;
  const seq=!!m.seq&&mode==='solo'&&goals.length>0;
  if(seq)goals.sort((a,b)=>(a.n||0)-(b.n||0)); /* ordre de la séquence (tri stable : à égalité, ordre de pose) */
  const C={name:m.name,small:!!m.small,defPads:m.defPads!==false,obs:obs,pads:pads,goals:goals,fins:fins,mode:mode,texW:texClean(m.tw),texF:texClean(m.tf),texR:TWR.some(q=>q[0]===+m.twr)?+m.twr:1,size:clamp(Math.round(+m.size||2),1,3),defGoals:(mode==='parkour'||seq)?false:m.defGoals!==false,gs:gs,seq:seq};
  if(seq)grSetup(C,goals.map(g=>({ring:g.sh==='ring',w:g.w,h:g.h,e:g.y0,bd:g.bd,by:g.by,hv:g.hv})),true,cmGoalHash(m.obj));
  return C;
}
/* empreinte des buts d'une séquence : sert de clé pour le record de la carte */
function cmGoalHash(objs){let h=5381;const t=JSON.stringify(cmSeqList(objs).map(g=>[g.x,g.z,g.e,g.w,g.h,g.d,g.yaw,g.sh,g.bd,g.by,g.hv]));for(let i=0;i<t.length;i++)h=((h<<5)+h+t.charCodeAt(i))|0;return (h>>>0).toString(36);}
function cmSeqList(objs){const L=(objs||[]).slice(0,160).map(cmClean).filter(o=>o&&o.t==='but').map((o,i)=>[o,i]);return L.sort((a,b)=>((a[0].n||0)-(b[0].n||0))||(a[1]-b[1])).map(q=>q[0]);}
/* physique : appelée depuis sdf() ; retire chaque solide de l'espace libre */
function cmSdf(px,py,pz,o){
  const L=CM.cur.obs;
  for(let i=0;i<L.length;i++){
    const q=L[i];if(q.off)continue;let dx=px-q.x,dy=py-q.y,dz=pz-q.z;
    if(Math.sqrt(dx*dx+dy*dy+dz*dz)-q.br>-o.d)continue;
    const x=dx*q.cy-dz*q.sy,z0=dx*q.sy+dz*q.cy;
    const y=dy*q.cp+z0*q.sp,z=-dy*q.sp+z0*q.cp;
    let d,nx,ny,nz;
    if(q.s==='box'){rbox(x,y,z,0,0,0,q.hx,q.hy,q.hz,Math.min(.25,q.hx,q.hy,q.hz),S1);d=S1.d;nx=S1.nx;ny=S1.ny;nz=S1.nz;}
    else if(q.s==='cyl'){const rl=Math.hypot(x,z)||1e-6,a=rl-q.r,b=Math.abs(y)-q.hy;
      if(a>0&&b>0){const l=Math.hypot(a,b);d=l;nx=a/l*x/rl;nz=a/l*z/rl;ny=b/l*(y<0?-1:1);}
      else if(a>b){d=a;nx=x/rl;nz=z/rl;ny=0;}else{d=b;nx=nz=0;ny=y<0?-1:1;}}
    else if(q.s==='sph'){const l=Math.sqrt(x*x+y*y+z*z)||1e-6;d=l-q.r;nx=x/l;ny=y/l;nz=z/l;}
    else{torusD(x,y,z,0,0,0,q.Rc,q.rt,S1);d=S1.d;nx=S1.nx;ny=S1.ny;nz=S1.nz;}
    const ny2=ny*q.cp-nz*q.sp,nz2=ny*q.sp+nz*q.cp;
    S1.d=d;S1.nx=nx*q.cy+nz2*q.sy;S1.ny=ny2;S1.nz=-nx*q.sy+nz2*q.cy;
    cutSolid(o);
  }
}
/* polygones : murs (sauf côtés ouverts) et disque horizontal */
function polyWallGeo(sh,N,Rc,h,skipOpen){
  const V=polyV(N,Rc),P=[],U=[];
  for(let k=0;k<N;k++){if(skipOpen&&polyOpen(sh,k,N))continue;const a=V[(k+N-1)%N],b=V[k],len=Math.hypot(b[0]-a[0],b[1]-a[1]);
    P.push(a[0],0,a[1],b[0],0,b[1],b[0],h,b[1],a[0],0,a[1],b[0],h,b[1],a[0],h,a[1]);U.push(0,0,len/2,0,len/2,h/2,0,0,len/2,h/2,0,h/2);}
  const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(P,3));g.setAttribute('uv',new T.Float32BufferAttribute(U,2));return g;
}
function polyFanGeo(N,Rc,y){
  const V=polyV(N,Rc),P=[];for(let k=0;k<N;k++){const a=V[k],b=V[(k+1)%N];P.push(0,y,0,a[0],y,a[1],b[0],y,b[1]);}
  const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(P,3));return g;
}
/* but libre : rendu (filets + montants) */
function cmBuildGoal(g){
  const colHex=CM.cur.mode==='solo'?parseInt(String(g.col).slice(1),16):[TEAM[0],TEAM[1],0x35e0a1][g.team];
  const gr=new T.Group(),w=g.w,h=g.h,d=g.d;
  if(g.sh==='ring'){ /* cerceau 2D : tore lumineux + disque translucide, axe = avant du but */
    const Rc=g.Rc,rt=g.rt;
    const tor=new T.Mesh(new T.TorusGeometry(Rc,rt,16,72),new T.MeshStandardMaterial({color:colHex,emissive:colHex,emissiveIntensity:.9,roughness:.35,metalness:.3}));tor.position.y=Rc;gr.add(tor);
    const disc=new T.Mesh(new T.CircleGeometry(Rc-rt,48),new T.MeshBasicMaterial({color:colHex,transparent:true,opacity:.14,side:T.DoubleSide,depthWrite:false}));disc.position.y=Rc;gr.add(disc);
    const edge=new T.Mesh(new T.TorusGeometry(Rc-rt,.1,8,72),new T.MeshBasicMaterial({color:0xffffff,transparent:true,opacity:.85}));edge.position.y=Rc;gr.add(edge);
    gr.rotation.y=g.yaw;gr.position.set(g.x,g.y0,g.z);arenaGroup.add(gr);return;}
  if(g.N){
    const N=g.N,Rc=g.Rc,V=polyV(N,Rc),t0=TX.net.clone();t0.needsUpdate=true;t0.wrapS=t0.wrapT=T.RepeatWrapping;
    const nm=new T.MeshBasicMaterial({map:t0,color:colHex,transparent:true,opacity:.85,side:T.DoubleSide,depthWrite:false});
    gr.add(new T.Mesh(polyWallGeo(g.sh,N,Rc,h,true),nm));
    const roof=polyFanGeo(N,Rc,h),rt=TX.net.clone();rt.needsUpdate=true;gr.add(new T.Mesh(roof,new T.MeshBasicMaterial({color:colHex,transparent:true,opacity:.22,side:T.DoubleSide,depthWrite:false})));
    gr.add(new T.Mesh(polyFanGeo(N,Rc,.04),new T.MeshStandardMaterial({color:0x101a30,emissive:colHex,emissiveIntensity:.22,roughness:.6,side:T.DoubleSide})));
    const gl=new T.MeshBasicMaterial({color:colHex}),ps=[];
    for(let k=0;k<N;k++)if(polyOpen(g.sh,k,N)!==polyOpen(g.sh,(k+1)%N,N)){const v=V[k];ps.push(v);const p=new T.Mesh(new T.BoxGeometry(.8,h+.8,.8),gl);p.position.set(v[0],h/2,v[1]);gr.add(p);}
    if(ps.length===2){const dx=ps[1][0]-ps[0][0],dz=ps[1][1]-ps[0][1],cb=new T.Mesh(new T.BoxGeometry(Math.hypot(dx,dz),.8,.8),gl);cb.position.set((ps[0][0]+ps[1][0])/2,h,(ps[0][1]+ps[1][1])/2);cb.rotation.y=-Math.atan2(dz,dx);gr.add(cb);}
    if(g.y0>.5){const pl=new T.Mesh(new T.CylinderGeometry(Rc+.8,Rc+.8,.8,N),new T.MeshStandardMaterial({color:0x131b34,emissive:colHex,emissiveIntensity:.3,roughness:.5}));pl.position.set(0,-.4,0);gr.add(pl);}
    gr.rotation.y=g.yaw;gr.position.set(g.x,g.y0,g.z);arenaGroup.add(gr);return;
  }
  const mk=(rx,ry)=>{const t=TX.net.clone();t.needsUpdate=true;t.wrapS=t.wrapT=T.RepeatWrapping;t.repeat.set(rx,ry);return new T.MeshBasicMaterial({map:t,color:colHex,transparent:true,opacity:.85,side:T.DoubleSide,depthWrite:false});};
  const back=new T.Mesh(new T.PlaneGeometry(w,h),mk(w/2,h/2));back.position.set(0,h/2,-d/2);gr.add(back);
  for(const sx of [-1,1]){const sd=new T.Mesh(new T.PlaneGeometry(d,h),mk(d/2,h/2));sd.rotation.y=Math.PI/2;sd.position.set(sx*w/2,h/2,0);gr.add(sd);}
  const top=new T.Mesh(new T.PlaneGeometry(w,d),mk(w/2,d/2));top.rotation.x=Math.PI/2;top.position.set(0,h,0);gr.add(top);
  const fl=new T.Mesh(new T.PlaneGeometry(w,d),new T.MeshStandardMaterial({color:0x101a30,emissive:colHex,emissiveIntensity:.22,roughness:.6,side:T.DoubleSide}));
  fl.rotation.x=-Math.PI/2;fl.position.y=.04;gr.add(fl);
  const gl=new T.MeshBasicMaterial({color:colHex});
  for(const sx of [-1,1]){const p=new T.Mesh(new T.BoxGeometry(.8,h+.8,.8),gl);p.position.set(sx*w/2,h/2,d/2);gr.add(p);}
  const cb=new T.Mesh(new T.BoxGeometry(w+.8,.8,.8),gl);cb.position.set(0,h,d/2);gr.add(cb);
  if(g.y0>.5){const pl=new T.Mesh(new T.BoxGeometry(w+1.6,.8,d+.8),new T.MeshStandardMaterial({color:0x131b34,emissive:colHex,emissiveIntensity:.3,roughness:.5}));pl.position.set(0,-.4,-.4);gr.add(pl);}
  gr.rotation.y=g.yaw;gr.position.set(g.x,g.y0,g.z);arenaGroup.add(gr);
}
/* but libre : la balle est dedans (tout son volume a passé la ligne) -> but */
function cgCheck(b){
  const C=CM.cur;
  for(let i=0;i<C.goals.length;i++){
    if(MODE.run&&i!==GR.lv)continue;
    const g=C.goals[i],dx=b.pos.x-g.x,dz=b.pos.z-g.z,dy=b.pos.y-g.y0;
    const lx=dx*g.cy-dz*g.sy,lz=dx*g.sy+dz*g.cy;
    let inside=false;
    if(g.sh==='ring'){ /* traversée du plan du cerceau (segment position précédente -> actuelle), dans le trou */
      const ly=dy-g.Rc,now=performance.now(),p=g.pv;g.pv={lx:lx,ly:ly,lz:lz,t:now};
      if(p&&now-p.t<250&&Math.hypot(lx-p.lx,ly-p.ly,lz-p.lz)<14){
        const fwd=p.lz>0&&lz<=0,bak=p.lz<=0&&lz>0;
        if(fwd||(bak&&C.mode==='solo')){const k=p.lz/(p.lz-lz);inside=Math.hypot(p.lx+(lx-p.lx)*k,p.ly+(ly-p.ly)*k)<g.Rc-g.rt;}
      }
    }else inside=g.N?(polyIn(g,lx,lz,BR*.5)&&dy>-BR*.5&&dy<g.h):(lz<g.d/2-BR*.85&&lz>-g.d/2-BR&&Math.abs(lx)<g.w/2&&dy>-BR*.5&&dy<g.h);
    if(inside){
      let team;
      if(C.mode==='solo')team=0;
      else if(g.team<2)team=1-g.team;
      else{if(!ball.last)continue;team=ball.last.team;}
      onGoal(team,V3(g.x,g.y0+(g.sh==='ring'?g.Rc:g.h*.45),g.z));return true;
    }
  }
  return false;
}
/* rendu : meshes + pastilles dans arenaGroup */
function cmBuild(){
  const C=CM.cur;
  if(!C.gr)C.goals.forEach(cmBuildGoal); /* parcours de buts : construits par grApply */
  C.fins.filter(f=>f.N).forEach(f=>{const gr=new T.Group(),wg=polyWallGeo(f.sh,f.N,f.Rc,f.h,false);
    gr.add(new T.Mesh(wg,new T.MeshBasicMaterial({color:0x35ff9a,transparent:true,opacity:.16,depthWrite:false,side:T.DoubleSide})));
    gr.add(new T.LineSegments(new T.EdgesGeometry(wg),new T.LineBasicMaterial({color:0xffffff})));
    gr.add(new T.Mesh(polyFanGeo(f.N,f.Rc,.06),new T.MeshBasicMaterial({color:0x35ff9a,transparent:true,opacity:.35,depthWrite:false,side:T.DoubleSide})));
    gr.rotation.y=f.yaw;gr.position.set(f.x,f.y0,f.z);arenaGroup.add(gr);});
  C.fins.filter(f=>!f.N).forEach(f=>{const gr=new T.Group(),bg=new T.BoxGeometry(f.w,f.h,f.d);
    gr.add(new T.Mesh(bg,new T.MeshBasicMaterial({color:0x35ff9a,transparent:true,opacity:.16,depthWrite:false,side:T.DoubleSide})));
    gr.add(new T.LineSegments(new T.EdgesGeometry(bg),new T.LineBasicMaterial({color:0xffffff})));
    const fl=new T.Mesh(new T.PlaneGeometry(f.w,f.d),new T.MeshBasicMaterial({color:0x35ff9a,transparent:true,opacity:.35,depthWrite:false}));fl.rotation.x=-Math.PI/2;fl.position.y=-f.h/2+.06;gr.add(fl);
    gr.rotation.y=f.yaw;gr.position.set(f.x,f.y0+f.h/2,f.z);arenaGroup.add(gr);});
  C.obs.forEach(q=>{
    if(q.hide)return;
    let g=q.s==='box'?new T.BoxGeometry(q.w,q.h,q.d):q.s==='cyl'?new T.CylinderGeometry(q.r,q.r,q.h,28):q.s==='sph'?new T.SphereGeometry(q.r,28,18):new T.TorusGeometry(q.Rc,q.rt,12,48);
    if(q.s==='tor')g.rotateX(Math.PI/2);
    const col=new T.Color(q.col);
    const m=new T.Mesh(g,new T.MeshStandardMaterial({color:col,roughness:.45,metalness:.25,emissive:col,emissiveIntensity:.22}));
    m.rotation.order='YXZ';m.rotation.set(q.pitch,q.yaw,0);m.position.set(q.x,q.y,q.z);
    if(q.s!=='sph'){const e=new T.LineSegments(new T.EdgesGeometry(g,30),new T.LineBasicMaterial({color:0xffffff,transparent:true,opacity:.45}));m.add(e);}
    arenaGroup.add(m);
  });
  C.pads.forEach(p=>{
    const g=new T.Group(),big=p.big,sz=big?7:3.6;
    const d=new T.Mesh(new T.PlaneGeometry(sz,sz),new T.MeshBasicMaterial({map:TX.pad,transparent:true,depthWrite:false,blending:T.AdditiveBlending}));
    d.rotation.x=-Math.PI/2;d.position.y=.06;g.add(d);let orb=null;
    if(big){orb=new T.Mesh(new T.IcosahedronGeometry(1.2,0),new T.MeshBasicMaterial({color:0xffc266}));orb.position.y=2.2;g.add(orb);
      const gs=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0xff9a30,transparent:true,blending:T.AdditiveBlending,depthWrite:false}));gs.scale.set(6,6,1);orb.add(gs);}
    g.position.set(p.x,0,p.z);arenaGroup.add(g);pads.push({x:p.x,z:p.z,big:big,active:true,t:0,g:g,orb:orb,d:d});
  });
}
/* =====================================================================
   PARCOURS DE BUTS : une suite de buts en l'air, de plus en plus hauts et petits.
   Un seul but est ouvert à la fois (les autres sont scellés) et la balle se place
   alignée devant lui. Le dernier « but » est un cerceau. Cerceau marqué = victoire.
   Pour ajouter un niveau : ajoute une ligne dans GR_LV (ordre = ordre de jeu).
   ===================================================================== */
const GR={on:false,lv:0,t:0,run:false,wait:0,pulse:0,goals:[],spots:[],levels:[],split:[],beacon:null,labels:[],KEY:'rocketArena3D.goalRunBest'};
/* s : mur (N = fond -z, S = fond +z, E = côté +x, W = côté -x) • p : position le long du mur • e : altitude du but
   w,h,d : largeur, hauteur, profondeur • bd : distance de la balle devant l'ouverture • by : altitude de la balle (0 = au sol)
   hv : la balle flotte sur place jusqu'à ce qu'on la touche */
const GR_LV=[
  {s:'N',p:0,  e:0, w:50,h:18,d:20,bd:46,by:0},
  {s:'S',p:-30,e:7, w:42,h:16,d:18,bd:44,by:5, hv:1},
  {s:'E',p:-35,e:12,w:36,h:15,d:16,bd:42,by:8, hv:1},
  {s:'W',p:40, e:16,w:30,h:14,d:15,bd:40,by:10,hv:1},
  {s:'N',p:55, e:20,w:26,h:12,d:13,bd:40,by:13,hv:1},
  {s:'S',p:35, e:24,w:22,h:11,d:12,bd:38,by:16,hv:1},
  {s:'E',p:45, e:27,w:18,h:10,d:10,bd:36,by:19,hv:1},
  {s:'W',p:-40,e:30,w:15,h:9, d:9, bd:36,by:22,hv:1},
  {ring:1,x:0,z:0,e:20,w:16,yaw:0,bd:34,by:24,hv:1} /* cerceau final, flottant au centre de l'arène */
];
const GR_COL={lock:'#ff4d5e',done:'#8794ad',act:'#35ff9a',ring:'#ffd23f'};
/* la carte « Parcours de buts » sous forme de carte du créateur (même format que les cartes de la communauté) */
function grMap(){
  const HXa=ARENAS.std.HX,HZa=ARENAS.std.HZ,obj=[];
  GR_LV.forEach((L,i)=>{
    if(L.ring){obj.push({t:'but',sh:'ring',x:L.x,z:L.z,e:L.e,w:L.w,h:L.w,d:L.w,yaw:L.yaw,pitch:0,c:GR_COL.lock,team:0,n:i+1,bd:L.bd,by:L.by||0,hv:L.hv?1:0});return;}
    const hd=L.d/2;let x=0,z=0,yaw=0;
    if(L.s==='N'){z=-HZa+hd+1;x=L.p;yaw=0;}
    else if(L.s==='S'){z=HZa-hd-1;x=L.p;yaw=180;}
    else if(L.s==='E'){x=HXa-hd-1;z=L.p;yaw=270;}
    else{x=-HXa+hd+1;z=L.p;yaw=90;}
    obj.push({t:'but',sh:'rect',x:x,z:z,e:L.e,w:L.w,h:L.h,d:L.d,yaw:yaw,pitch:0,c:GR_COL.lock,team:0,n:i+1,bd:L.bd,by:L.by||0,hv:L.hv?1:0});
  });
  return {name:'Parcours de buts',mode:'solo',seq:true,obj:obj,defGoals:false,defPads:false,small:false,size:1,gs:1};
}
function grBH(){return GR._bh||(GR._bh=cmGoalHash(grMap().obj));}
function grCompile(){
  const C=cmCompile(grMap());
  C.gr.key=GR.KEY; /* le record historique du mode reste dans sa clé d'origine */
  return C;
}
/* prépare une carte à séquence : un sceau par but (bouche l'ouverture tant que le but n'est pas actif) + le point d'apparition de la balle et de la voiture.
   lv : un descripteur par but {ring,w,h,e,bd,by,hv} • custom : carte du créateur (les points sont ramenés dans l'arène) */
function grSetup(C,lv,custom,hash){
  C.gr={seals:[],spots:[],lv:lv,key:(hash&&hash===grBH())?GR.KEY:'rocketArena3D.goalRunBest.'+(hash||'x')}; /* même parcours que l'officiel : on garde l'ancien record */
  const hx=C.small?40:96,hz=C.small?60:120;
  C.goals.forEach((g,i)=>{
    const L=lv[i],ring=g.sh==='ring';
    /* sceau : bouche l'ouverture tant que le but n'est pas actif (physique, pour la balle comme pour les voitures) */
    let q;
    if(ring){const r=g.Rc-g.rt*.5;q={t:'but',s:'cyl',hide:true,seal:i,off:false,x:g.x,y:g.y0+g.Rc,z:g.z,cy:g.cy,sy:g.sy,cp:0,sp:1,yaw:g.yaw,pitch:Math.PI/2,col:GR_COL.lock,hx:r,hy:.35,hz:r,r:r,Rc:r,rt:.35,w:r*2,h:.7,d:r*2,br:Math.hypot(r,.35)};}
    else q={t:'but',s:'box',hide:true,seal:i,off:false,x:g.mx,y:g.y0+g.h/2,z:g.mz,cy:g.cy,sy:g.sy,cp:1,sp:0,yaw:g.yaw,pitch:0,col:GR_COL.lock,hx:g.w/2,hy:g.h/2,hz:.35,r:g.w/2,Rc:g.w/2,rt:.35,w:g.w,h:g.h,d:.7,br:Math.hypot(g.w/2,g.h/2,.35)};
    C.obs.push(q);C.gr.seals.push(q);
    /* point d'apparition : la balle sur l'axe du but, la voiture derrière elle, face au but */
    const mx=ring?g.x:g.mx,mz=ring?g.z:g.mz;let bx=mx+g.fx*L.bd,bz=mz+g.fz*L.bd,cx=bx+g.fx*16,cz=bz+g.fz*16;
    if(custom){bx=clamp(bx,-hx+8,hx-8);bz=clamp(bz,-hz+8,hz-8);cx=clamp(cx,-hx+6,hx-6);cz=clamp(cz,-hz+6,hz-6);}
    C.gr.spots.push({bx:bx,bz:bz,by:L.by||0,hv:!!L.hv,cx:cx,cz:cz,a:Math.atan2(-g.fx,-g.fz)});
  });
}
function grKey(){return (CM.cur&&CM.cur.gr&&CM.cur.gr.key)||GR.KEY;}
function grBest(){try{return JSON.parse(localStorage.getItem(grKey()))||{};}catch(e){return {};}}
/* appelé juste après la construction de l'arène */
function grInit(){
  const C=CM.cur;
  GR.on=true;GR.lv=0;GR.t=0;GR.run=false;GR.wait=0;GR.pulse=0;GR.split=[];GR.goals=C.goals;GR.spots=C.gr.spots;GR.levels=C.gr.lv;
  GR.labels=[];
  const lb=new T.Sprite(new T.SpriteMaterial({map:TX.glow,color:0x35ff9a,transparent:true,blending:T.AdditiveBlending,depthWrite:false,depthTest:false,opacity:.8}));
  lb.scale.set(24,24,1);lb.renderOrder=9;arenaGroup.add(lb);GR.beacon=lb;
  C.goals.forEach((g,i)=>{
    const ring=g.sh==='ring',top=g.y0+(ring?g.Rc*2:g.h);
    const lab=makeLabel(ring?'CERCEAU':'BUT '+(i+1),'#ffffff');lab.scale.set(11,2.75,1);
    lab.position.set(g.x,top+3.2,g.z);arenaGroup.add(lab);
    g.st=-1;g.grp=null;g.panel=null;
  });
  grApply();
}
/* état visuel + physique de chaque but : 0 = verrouillé, 1 = actif, 2 = terminé */
function grApply(){
  const C=CM.cur;if(!C||!C.gr)return;
  C.gr.seals.forEach((q,i)=>{q.off=(i===GR.lv);});
  C.goals.forEach((g,i)=>{
    const st=i<GR.lv?2:(i===GR.lv?1:0),ring=g.sh==='ring';
    if(g.st===st&&g.grp)return;
    g.st=st;
    if(g.grp){arenaGroup.remove(g.grp);g.grp.traverse(m=>{if(m.geometry)m.geometry.dispose();});}
    if(g.panel){arenaGroup.remove(g.panel);g.panel.geometry.dispose();g.panel=null;}
    g.col=st===2?GR_COL.done:(st===1?(ring?GR_COL.ring:GR_COL.act):GR_COL.lock);
    cmBuildGoal(g);g.grp=arenaGroup.children[arenaGroup.children.length-1];
    if(st!==1){
      const colN=parseInt(g.col.slice(1),16);
      const geo=ring?new T.CircleGeometry(g.Rc-g.rt*.5,40):new T.PlaneGeometry(g.w,g.h);
      const pn=new T.Mesh(geo,new T.MeshBasicMaterial({color:colN,transparent:true,opacity:st===0?.34:.16,side:T.DoubleSide,depthWrite:false}));
      pn.rotation.y=g.yaw;
      if(ring)pn.position.set(g.x,g.y0+g.Rc,g.z);else pn.position.set(g.mx,g.y0+g.h/2,g.mz);
      arenaGroup.add(pn);g.panel=pn;
    }
  });
  const g=C.goals[GR.lv],ring=g.sh==='ring';
  GR.beacon.position.set(g.x,g.y0+(ring?g.Rc:g.h/2),g.z);
  GR.beacon.material.color.set(ring?0xffd23f:0x35ff9a);
  grHud();
}
function grHud(){
  const n=GR.levels.length,L=GR.levels[GR.lv],ring=!!L.ring;
  let pips='';for(let i=0;i<n;i++)pips+='<i class="'+(i<GR.lv?'d':(i===GR.lv?(ring?'r':'a'):''))+'"></i>';
  $('grHud').innerHTML='<b>'+(ring?'⭕ CERCEAU FINAL':'🎯 BUT '+(GR.lv+1)+' / '+n)+'</b><small>'+(ring?'Fais passer la balle dans le cerceau':'Ouverture '+L.w+' × '+L.h+' • altitude '+L.e)+' • R : replacer</small><div class="pips">'+pips+'</div>';
}
function grBallPos(){
  const s=GR.spots[GR.lv];if(!s)return;
  ball.pos.set(s.bx,s.by||(BR+.05),s.bz);ball.vel.set(0,0,0);
  ball.frozen=s.hv?9999:0;ball.py=ball.pos.y;ball.mesh.position.copy(ball.pos);
}
/* place la voiture et la balle devant le but actif */
function grSpawn(){
  if(!player||!GR.on)return;
  GR.wait=0;grApply();
  const s=GR.spots[GR.lv];
  placeCar(player);
  mvPlace(player,s.cx,CR+.02,s.cz,s.a,0);
  resetBall();clearSkids();
  GR.run=false;
}
function grGoal(gp){
  if(!GR.on||GR.wait>0||phase!=='play')return;
  const i=GR.lv,n=GR.levels.length,last=i>=n-1;
  GR.split.push(GR.t);GR.run=false;
  ball.alive=false;ball.mesh.visible=false;
  const ed=player?player.ed:IT['expl:0'].ed;
  runExplosion(ed,0,gp);SFX.goal();addShake(last?1:.6);
  el.flash.style.transition='none';el.flash.style.opacity='.5';void el.flash.offsetWidth;el.flash.style.transition='opacity .8s';el.flash.style.opacity='0';
  if(last){grWin(gp);return;}
  GR.lv=i+1;GR.wait=2;
  const nx=GR.levels[GR.lv];
  showMsg('BUT '+(i+1)+' / '+n+' !',nx.ring?'Dernière étape : le cerceau…':'Prochain but : '+nx.w+' × '+nx.h+', altitude '+nx.e,'#7dffb0',2);
}
function grWin(gp){
  phase='over';phaseT=0;clearMsg();
  const b=grBest(),old=b.best,rec=!(old<=GR.t);
  if(rec){b.best=+GR.t.toFixed(2);try{localStorage.setItem(grKey(),JSON.stringify(b));}catch(e){}}
  for(let k=0;k<5;k++)runExplosion(player.ed,0,V3(gp.x+(rnd()-.5)*22,gp.y+(rnd()-.5)*12,gp.z+(rnd()-.5)*22));
  crowdRoar();
  el.endT.textContent='VICTOIRE !';el.endT.style.color='#7dffb0';
  el.endS.textContent='Parcours terminé en '+pkFmt(GR.t)+(rec?'  •  NOUVEAU RECORD':'  •  record : '+pkFmt(old));
  let rows='',prev=0;
  GR.split.forEach((t,i)=>{rows+='<tr><td>'+(GR.levels[i]&&GR.levels[i].ring?'Cerceau':'But '+(i+1))+'</td><td>'+pkFmt(t)+'</td><td>+'+(t-prev).toFixed(2)+' s</td></tr>';prev=t;});
  el.endTable.innerHTML='<table><tr><th>ÉTAPE</th><th>TEMPS</th><th>SPLIT</th></tr>'+rows+'</table>';
  $('endRew').innerHTML='';$('again').textContent='Rejouer';
  el.end.classList.add('show');el.touch.classList.add('hide');
}
function runUpdate(dt){
  if(!GR.on||!player)return;
  if(phase==='play'){
    if(GR.wait>0){GR.wait-=dt;if(GR.wait<=0)grSpawn();}
    else{
      if(!GR.run&&(player.vel.length()>1.5||player.in.thr!==0))GR.run=true;
      if(GR.run)GR.t+=dt;
    }
  }
  GR.pulse+=dt;
  if(GR.beacon){const k=1+.12*Math.sin(GR.pulse*4);GR.beacon.scale.set(24*k,24*k,1);GR.beacon.material.opacity=.55+.25*Math.sin(GR.pulse*4);}
}

/* ---------- stockage ---------- */
function cmLoadAll(){try{return JSON.parse(localStorage.getItem(CM.KEY))||[];}catch(e){return [];}}
function cmSaveAll(l){try{localStorage.setItem(CM.KEY,JSON.stringify(l));}catch(e){toast('Stockage plein');}}
/* textures personnalisées d'une carte (murs / sol) : images réduites, stockées dans la carte (data URL) */
const TWR=[[2,'Petit'],[1,'Normal'],[.35,'Grand']];
function texClean(u){return (typeof u==='string'&&u.length<=230000&&/^data:image\/(webp|png|jpeg);base64,[A-Za-z0-9+\/=]+$/.test(u))?u:'';}
function imgPack(file,max,done){ /* fichier image -> data URL webp d'au plus max px et ~150 Ko */
  if(!file||!/^image\//.test(file.type)){done('');return;}
  const r=new FileReader();r.onerror=()=>done('');
  r.onload=()=>{const im=new Image();im.onerror=()=>done('');
    im.onload=()=>{let m=max,u='';
      for(let i=0;i<5;i++){const k=Math.min(1,m/Math.max(im.width,im.height)),w=Math.max(1,Math.round(im.width*k)),h=Math.max(1,Math.round(im.height*k));
        const [c,x]=mkc(w,h);x.drawImage(im,0,0,w,h);u=c.toDataURL('image/webp',.78-i*.06);if(u.length<=150000)break;m=Math.round(m*.7);}
      done(texClean(u));};
    im.src=r.result;};
  r.readAsDataURL(file);
}
function cmTex(url,rep){const im=new Image(),t=new T.Texture(im);t.wrapS=t.wrapT=T.RepeatWrapping;t.anisotropy=Math.min(8,renderer.capabilities.getMaxAnisotropy?renderer.capabilities.getMaxAnisotropy():1);
  t.repeat.set(rep||1,rep||1);im.onload=()=>{t.needsUpdate=true;};im.src=url;return t;}
const _cmImgs={};
function cmImg(u){if(!u)return null;let i=_cmImgs[u];if(!i){i=_cmImgs[u]=new Image();i.onload=()=>{try{cmDraw();}catch(e){}};i.src=u;}return i.complete&&i.naturalWidth?i:null;}
function cmNew(){return {name:'Ma carte',small:false,defPads:true,sym:true,obj:[],mode:'bots',size:2,defGoals:true,gs:1,tw:'',tf:'',twr:1,seq:false};}
/* ---------- séquence de buts (carte solo) : les buts s'ouvrent un par un, dans l'ordre 1, 2, 3… ---------- */
function cmGoalsSorted(m){const L=m.obj.filter(o=>o.t==='but');return L.map((o,i)=>[o,i]).sort((a,b)=>((a[0].n||0)-(b[0].n||0))||(a[1]-b[1])).map(q=>q[0]);}
function cmRenumber(m){cmGoalsSorted(m).forEach((o,i)=>{o.n=i+1;});}
function cmNextN(m){return m.obj.reduce((a,o)=>o.t==='but'?Math.max(a,o.n|0):a,0)+1;}
function cmSeqFix(m){m.obj.forEach(o=>{if(o.t!=='but')return;if(o.bd===undefined)o.bd=40;if(o.by===undefined)o.by=0;if(o.hv===undefined)o.hv=0;});cmRenumber(m);}
/* normalise les réglages d'une carte venue du stockage / d'un import (anciennes cartes : type « bots », cages classiques) */
function cmFrom(raw){const m=Object.assign(cmNew(),raw||{});
  m.mode=(m.mode==='solo'||m.mode==='parkour')?m.mode:'bots';m.size=clamp(Math.round(+m.size||2),1,3);m.defGoals=m.defGoals!==false;
  m.seq=m.mode==='solo'&&!!m.seq;if(m.seq){m.defGoals=false;if(Array.isArray(m.obj))cmSeqFix(m);}m.gs=GSC.some(q=>q[0]===+m.gs)?+m.gs:1;
  m.tw=texClean(m.tw);m.tf=texClean(m.tf);m.twr=TWR.some(q=>q[0]===+m.twr)?+m.twr:1;
  if(!Array.isArray(m.obj))m.obj=[];return m;}
let _cmId=1;
function cmMirrorTeam(t){return t===2?2:1-(t|0);}
function cmAdd(ed,type,x,z,extra){
  const t=CMT[type],m=ed.map;
  const o=Object.assign({id:_cmId++,t:type,x:x,z:z,e:t.e,w:t.v[0],h:t.v[1],d:t.v[2],yaw:0,pitch:t.p,c:CMCOL[ed.col%CMCOL.length]},extra||{});
  if(t.s==='goal'){
    if(!extra||extra.team===undefined)o.team=Math.abs(z)<8?2:(z<0?0:1);       /* moitié bleue / orange, centre = neutre */
    if(!extra||extra.yaw===undefined)o.yaw=((Math.round(Math.atan2(-x,-z)*180/Math.PI/15)*15)+360)%360; /* ouverture tournée vers le centre */
  }
  if(m.obj.length>=160){toast('Maximum : 160 éléments');return null;}
  if(t.s==='goal'&&m.seq){if(!(o.n>0))o.n=cmNextN(m);if(o.bd===undefined)o.bd=40;if(o.by===undefined)o.by=0;if(o.hv===undefined)o.hv=0;}
  m.obj.push(o);
  if(m.sym&&!m.seq&&t.s!=='fin'&&(Math.abs(x)>1||Math.abs(z)>1)){const w=Object.assign({},o,{id:_cmId++,x:-x,z:-z,yaw:(o.yaw+180)%360});if(t.s==='goal')w.team=cmMirrorTeam(o.team);o.tw=w.id;w.tw=o.id;m.obj.push(w);}
  return o;
}
function cmSync(ed,o){const t=ed.map.obj.find(q=>q.id===o.tw);if(t){Object.assign(t,{t:o.t,w:o.w,h:o.h,d:o.d,e:o.e,pitch:o.pitch,c:o.c,x:-o.x,z:-o.z,yaw:(o.yaw+180)%360});if(o.t==='but')t.team=cmMirrorTeam(o.team);t.sh=o.sh;}}
/* zones d'apparition des voitures (1v1 à 3v3) : on n'y pose pas de but */
function cmSpawnPts(m){const sx=m.small?1:96/40,sz=m.small?1:120/60,pts=[],solo=cmAlone(m),n=solo?1:clamp(+m.size||2,1,3);
  SP[n].forEach(p=>{pts.push([p[0]*sx,-p[1]*sz]);if(!solo)pts.push([-p[0]*sx,p[1]*sz]);});return pts;} /* bleu : (+x,-z) / orange : (-x,+z) */
function cmOk(ed,x,z,t){const [hx,hz]=ed.map.small?[40,60]:[96,120],k=ed.map.small?.45:1;
  if(Math.hypot(x,z)<8*k+2)return false;                                   /* rond central : engagement */
  if(ed.map.defGoals!==false&&ed.map.mode!=='parkour'&&Math.abs(x)<(ed.map.small?10:21)*(ed.map.gs||1)+2&&Math.abs(z)>hz-(ed.map.small?14:24))return false; /* devant les cages */
  if(t&&t.t==='but'){const r=Math.hypot(t.w/2,t.d/2)+6;if(cmSpawnPts(ed.map).some(p=>Math.hypot(p[0]-x,p[1]-z)<r))return false;}
  const e=cmExt(ed.map);return Math.abs(x)<hx+e&&Math.abs(z)<hz+e;}
/* ---------- cartes modèles ---------- */
const CMP={
  vide:{n:'Vide',f:()=>{}},
  pyramide:{n:'Pyramide',f:(ed)=>{[[26,3],[18,9],[10,15]].forEach(q=>cmAdd(ed,'dalle',0,0,{w:q[0]*1.6,d:q[0]*1.6,h:6,e:q[1]-6+3-3}));}},
  couloirs:{n:'Couloirs',f:(ed)=>{[-48,-24,24,48].forEach(x=>[-60,0,60].forEach(z=>{if(Math.abs(z)<1&&Math.abs(x)<30)return;const o=cmAdd(ed,'mur',x,z,{d:2.5,w:2.5,h:10,yaw:0});if(o){o.d=26;cmSync(ed,o);}}));}},
  piliers:{n:'Forêt de piliers',f:(ed)=>{for(let i=0;i<14;i++){const x=(i%5-2)*30+((i*7)%5-2)*3,z=(Math.floor(i/5)-1)*44+10;if(cmOk(ed,x,z))cmAdd(ed,'pilier',x,z);}}},
  tremplins:{n:'Tremplins',f:(ed)=>{[[-55,-70],[55,-70],[0,-35],[-30,30]].forEach(q=>cmAdd(ed,'rampe',q[0],q[1],{yaw:q[0]>0?-20:20}));cmAdd(ed,'pont',0,0,{e:14,yaw:90,d:70});}},
  anneaux:{n:'Anneaux & dômes',f:(ed)=>{[[-50,-60],[50,-30],[0,70]].forEach(q=>{cmAdd(ed,'anneau',q[0],q[1],{yaw:q[1]>0?0:90});});[[-60,20],[60,-20]].forEach(q=>cmAdd(ed,'dome',q[0],q[1]));[[0,-90],[0,90]].forEach(q=>cmAdd(ed,'bpad',q[0],q[1]));}}
};
function cmRandom(ed){
  const ks=['bloc','mur','rampe','rampe2','dalle','pilier','plot','dome','boule','anneau','cerceau','pad','bpad','cube'];
  ed.map.obj=[];const [hx,hz]=cmDims().map(v=>v*.88);
  for(let i=0;i<14;i++){const t=ks[Math.floor(rnd()*ks.length)],x=Math.round((rnd()*2-1)*hx/2)*2,z=Math.round(rnd()*hz/2)*2;
    if(!cmOk(ed,x,z))continue;const o=cmAdd(ed,t,x,z,{yaw:Math.round(rnd()*12)*15,c:CMCOL[Math.floor(rnd()*CMCOL.length)]});}
}
/* ---------- écran éditeur ---------- */
const ED={map:cmNew(),sel:null,tool:'sel',col:0,zoom:1,px:0,pz:0,panMode:false,drag:null,grid:2}; /* zoom / px,pz : zoom et centre de la vue du dessus */
function rMaker(){
  if(!ED.init){ED.init=1;const l=cmLoadAll();if(l.length&&l[l.length-1])ED.map=cmFrom(JSON.parse(JSON.stringify(l[l.length-1])));ED.map.obj.forEach(o=>{if(o.id>=_cmId)_cmId=o.id+1;});}
  if(!document.getElementById('cmCss')){const s=document.createElement('style');s.id='cmCss';s.textContent=
   '.cmk{display:flex;flex-direction:row;gap:10px;padding:8px 10px!important}.cmk .cvw{flex:1;min-width:0;display:flex;flex-direction:column}.cmk canvas{flex:1;min-height:0;width:100%;background:#0a1330;border:2px solid #2f8bff;touch-action:none}'+
   '.cmk .side{width:min(46vw,380px);overflow:auto;-webkit-overflow-scrolling:touch}.cmk .pal{display:grid;grid-template-columns:repeat(3,1fr);gap:5px}'+
   '.cmk button.t{border:1px solid rgba(255,255,255,.2);background:#17234a;color:#fff;font:700 13px Rajdhani,system-ui;padding:6px 3px;cursor:pointer;line-height:1.1}.cmk button.t.on{background:#ff8a1f;color:#1a0d00;border-color:#fff}'+
   '.cmk .row{display:flex;align-items:center;gap:6px;margin:3px 0;font:700 13px Rajdhani,system-ui}.cmk .row span{flex:1}.cmk .row b{min-width:42px;text-align:center}.cmk .row button{width:34px;height:30px}.cmk h4{margin:8px 0 4px;font-size:11px;letter-spacing:2px;opacity:.7}'+
   '@media(max-width:760px) and (min-height:521px){.cmk{flex-direction:column}.cmk .side{width:auto;height:46%}.cmk .pal{grid-template-columns:repeat(5,1fr)}}';document.head.appendChild(s);}
  const solo=ED.map.mode==='solo',park=ED.map.mode==='parkour',alone=solo||park,seq=solo&&!!ED.map.seq;
  if((ED.tool==='arrivee'&&!park)||(ED.tool==='but'&&park))ED.tool='sel';
  const pal=Object.keys(CMT).filter(k=>CMT[k].s==='fin'?park:(CMT[k].s==='goal'?!park:true)).map(k=>'<button class="t'+(ED.tool===k?' on':'')+'" data-t="'+k+'">'+CMT[k].ic+'<br>'+CMT[k].n+'</button>').join('');
  $scr.innerHTML='<div class="rpanel cmk"><div class="cvw"><canvas id="cmC"></canvas><div class="row"><span id="cmInfo"></span><button class="t" id="cmZo" title="Dézoomer">－</button><button class="t" id="cmZi" title="Zoomer">＋</button><button class="t" id="cmZr" title="Recentrer la vue">⌂</button><button class="t'+(ED.panMode?' on':'')+'" id="cmPan" title="Déplacer la vue avec un doigt">🖐</button></div></div><div class="side">'+
   '<h4>TYPE DE CARTE</h4><div class="pal" style="grid-template-columns:1fr 1fr 1fr"><button class="t'+(solo?' on':'')+'" id="cmMdS">🧍 Solo<br><small>tirs</small></button><button class="t'+(park?' on':'')+'" id="cmMdP">🏁 Parkour<br><small>sans balle</small></button><button class="t'+(!alone?' on':'')+'" id="cmMdB">🤖 Bots<br><small>match</small></button></div>'+
   '<div class="row"><span style="opacity:.7;font-size:12px;font-weight:600">'+(park?'Parkour : carte ouverte (sans murs ni plafond), tu tombes dans le vide hors du sol. Pas de balle, boost illimité, chrono dès que tu bouges. Pose une Arrivée 🏁 ; R = recommencer. Tu peux aussi poser des éléments HORS du sol, dans le vide (zone pointillée) : parfait pour les cartes piégées (faux chemins, plateformes cachées…).':solo?'Tu joues seul, boost illimité. Chaque but compte pour toi.':'Match contre des bots. Chaque but appartient à une équipe (il rapporte un point à l\'autre).')+'</span></div>'+
   (solo?'<div class="row"><span>🎯 Séquence de buts</span><button class="t'+(seq?' on':'')+'" id="cmSeq" style="width:auto;padding:6px 10px">'+(seq?'OUI':'NON')+'</button></div>'+
     (seq?'<div class="row"><span style="opacity:.7;font-size:12px;font-weight:600">Parcours de buts : les buts s\'ouvrent un par un, dans l\'ordre (1, 2, 3…). Un seul but est ouvert à la fois et la balle se place devant lui. Dernier but marqué = victoire, avec chrono et temps intermédiaires. Pose tes buts (cage 3D ou cerceau 2D) puis règle l\'ordre, la distance et l\'altitude de la balle dans les propriétés de chaque but. R = replacer la balle.</span></div>':''):'')+
   '<h4>OUTIL</h4><div class="pal"><button class="t'+(ED.tool==='sel'?' on':'')+'" data-t="sel">✋<br>Sélection</button>'+pal+'</div><div id="cmProp"></div>'+
   '<h4>CARTE</h4><div class="row"><span>Nom</span><button class="t" id="cmName" style="width:auto;padding:6px 10px">'+esc(ED.map.name)+'</button></div>'+
   '<div class="row"><span>Taille</span><button class="t" id="cmSz" style="width:auto;padding:6px 10px">'+(ED.map.small?'Petite':'Standard')+'</button></div>'+
   (seq?'':'<div class="row"><span>Symétrie 180° (équité)</span><button class="t'+(ED.map.sym?' on':'')+'" id="cmSym" style="width:auto;padding:6px 10px">'+(ED.map.sym?'OUI':'NON')+'</button></div>')+
   '<div class="row"><span>Boosts classiques</span><button class="t'+(ED.map.defPads!==false?' on':'')+'" id="cmDp" style="width:auto;padding:6px 10px">'+(ED.map.defPads!==false?'OUI':'NON')+'</button></div>'+
   '<h4>TEXTURES (IMAGES)</h4>'+
   (park?'':cmTexRows('Murs','tw',ED.map.tw)+(ED.map.tw?'<div class="row"><span>Motif des murs</span><button class="t" id="cmTwR" style="width:auto;padding:6px 10px">'+TWR.find(q=>q[0]===ED.map.twr)[1]+'</button></div>':''))+
   cmTexRows('Sol','tf',ED.map.tf)+'<input type="file" id="cmFile" accept="image/*" style="display:none">'+
   '<div class="row"><span style="opacity:.6;font-size:11px;font-weight:600">Le sol est une image étirée sur tout le terrain. Les textures sont enregistrées avec la carte et publiées avec elle.</span></div>'+
   (park||seq?'':'<div class="row"><span>Cages classiques</span><button class="t'+(ED.map.defGoals!==false?' on':'')+'" id="cmDg" style="width:auto;padding:6px 10px">'+(ED.map.defGoals!==false?'OUI':'NON')+'</button></div>')+
   (!park&&!seq&&ED.map.defGoals!==false?'<div class="row"><span>Taille des cages</span><button class="t" id="cmGs" style="width:auto;padding:6px 10px">'+GSC.find(q=>q[0]===ED.map.gs)[1]+'</button></div>':'')+
   (alone?'':'<div class="row"><span>Joueurs par équipe</span>'+[1,2,3].map(n=>'<button class="t'+(ED.map.size===n?' on':'')+'" data-sz="'+n+'" style="width:44px">'+n+'v'+n+'</button>').join('')+'</div>')+
   '<h4>MODÈLES</h4><div class="pal">'+Object.keys(CMP).map(k=>'<button class="t" data-p="'+k+'">'+CMP[k].n+'</button>').join('')+'<button class="t" data-p="_rnd">🎲 Aléatoire</button><button class="t" data-p="_clr">🗑️ Tout effacer</button></div>'+
   '<h4>SAUVEGARDE</h4><div class="pal"><button class="t" id="cmSave">💾 Enregistrer</button><button class="t" id="cmOpen">📂 Mes cartes</button><button class="t" id="cmExp">📤 Exporter</button><button class="t" id="cmImp">📥 Importer</button><button class="t" id="cmPub" style="grid-column:1/-1">🌍 Publier à la communauté</button><button class="t" id="cmPriv" style="grid-column:1/-1">🔒 Carte privée (avec un code)</button></div>'+
   (park?'<button class="go" id="cmPlay">🏁 JOUER LE PARKOUR</button>':solo?'<button class="go" id="cmPlay">'+(seq?'🎯 JOUER LA SÉQUENCE':'▶ JOUER EN SOLO')+'</button>':'<button class="go" id="cmPlay">⚽ MATCH CONTRE BOTS</button><button class="go alt" id="cmTest">🛠 TESTER SEUL (LIBRE)</button>')+'</div></div>';
  cmWire();cmDraw();cmProps();
}
/* quitter le parkour : l'arène redevient fermée, les éléments posés dans le vide sont retirés */
function cmLeavePark(ed){const m=ed.map;if(m.mode!=='parkour')return;const [hx,hz]=cmDims(),n=m.obj.length;
  m.obj=m.obj.filter(o=>Math.abs(o.x)<hx&&Math.abs(o.z)<hz);if(m.obj.length<n){ed.sel=null;toast((n-m.obj.length)+' élément(s) hors du sol retiré(s) (arène fermée)');}}
function cmWire(){
  const cv=$('cmC'),ed=ED;
  const fit=()=>{const r=cv.getBoundingClientRect();cv.width=Math.max(10,r.width*devicePixelRatio);cv.height=Math.max(10,r.height*devicePixelRatio);cmDraw();};
  setTimeout(fit,30);window.onresize=()=>{if(UI.scr==='maker')fit();};
  /* vue du dessus : zoom (molette, pincement, boutons) et déplacement (glisser dans le vide, 2 doigts, clic droit / molette, bouton 🖐) */
  const K=()=>cmScale(cv)*ed.zoom;
  const SP=e=>{const r=cv.getBoundingClientRect();return {x:(e.clientX-r.left)*devicePixelRatio,y:(e.clientY-r.top)*devicePixelRatio};};
  const P=e=>{const s=SP(e),k=K();return {x:(s.x-cv.width/2)/k+ed.px,z:(s.y-cv.height/2)/k+ed.pz};};
  const lim=()=>{const [hx,hz]=cmDims(),x=cmExt(ed.map)+40;ed.px=clamp(ed.px,-hx-x,hx+x);ed.pz=clamp(ed.pz,-hz-x,hz+x);};
  const zoomAt=(sx,sy,f)=>{const k0=K(),wx=(sx-cv.width/2)/k0+ed.px,wz=(sy-cv.height/2)/k0+ed.pz;ed.zoom=clamp(ed.zoom*f,.5,10);const k1=K();ed.px=wx-(sx-cv.width/2)/k1;ed.pz=wz-(sy-cv.height/2)/k1;lim();cmDraw();};
  const snap=v=>Math.round(v/ed.grid)*ed.grid;
  const hit=p=>{const o=ed.map.obj;for(let i=o.length-1;i>=0;i--){const q=o[i],t=CMT[q.t],dx=p.x-q.x,dz=p.z-q.z;
      if(t.s==='pad'){if(Math.hypot(dx,dz)<q.w*.7)return q;continue;}
      const c=Math.cos(q.yaw*Math.PI/180),s=Math.sin(q.yaw*Math.PI/180),lx=dx*c-dz*s,lz=dx*s+dz*c;
      if((t.s==='goal'||t.s==='fin')&&shOf(q)!=='rect'){if(Math.hypot(dx,dz)<=q.w/2)return q;}
      else if(t.s==='box'||t.s==='goal'||t.s==='fin'){if(Math.abs(lx)<=q.w/2&&Math.abs(lz)<=q.d/2)return q;}else if(Math.hypot(dx,dz)<=q.w/2+1)return q;}return null;};
  const pts=new Map();let pinch=null,pan=null,pend=null;
  const place=p=>{const x=snap(p.x),z=snap(p.z);if(!cmOk(ed,x,z,ed.tool==='but'?CMT.but&&{t:'but',w:CMT.but.v[0],d:CMT.but.v[2]}:null)){toast(ed.map.mode==='parkour'?'Zone réservée ou trop loin':'Zone réservée (engagement / cages / apparition)');return;}
      const o=cmAdd(ed,ed.tool,x,z);
      if(o){ed.sel=o;if(o.t==='but'&&ed.map.defGoals!==false){ed.map.defGoals=false;toast('Cages classiques désactivées (tu as posé un but)');rMaker();return;}cmProps();cmDraw();}};
  cv.oncontextmenu=e=>e.preventDefault();
  cv.addEventListener('wheel',e=>{e.preventDefault();const s=SP(e);zoomAt(s.x,s.y,Math.exp(-e.deltaY*.0015));},{passive:false});
  cv.onpointerdown=e=>{try{cv.setPointerCapture(e.pointerId);}catch(_){}pts.set(e.pointerId,SP(e));
    if(pts.size===2){ /* deux doigts : pincement (zoom) + déplacement ; on annule ce qui était en cours */
      ed.drag=null;pend=null;pan=null;const [a,b]=[...pts.values()];pinch={d:Math.hypot(a.x-b.x,a.y-b.y)||1,mx:(a.x+b.x)/2,my:(a.y+b.y)/2};return;}
    if(pts.size>2)return;
    const s=SP(e),p=P(e);
    if(e.button===1||e.button===2||ed.panMode){pan={x:s.x,y:s.y};return;}
    if(ed.tool!=='sel'){pend={x:e.clientX,y:e.clientY};return;} /* pose au relâchement (permet de pincer / glisser sans rien poser) */
    const q=hit(p);ed.sel=q;if(q)ed.drag={dx:p.x-q.x,dz:p.z-q.z};else pan={x:s.x,y:s.y};cmProps();cmDraw();};
  cv.onpointermove=e=>{if(!pts.has(e.pointerId))return;const s=SP(e);pts.set(e.pointerId,s);
    if(pinch&&pts.size===2){const [a,b]=[...pts.values()],d=Math.hypot(a.x-b.x,a.y-b.y)||1,mx=(a.x+b.x)/2,my=(a.y+b.y)/2;
      ed.px-=(mx-pinch.mx)/K();ed.pz-=(my-pinch.my)/K();zoomAt(mx,my,d/pinch.d);pinch={d:d,mx:mx,my:my};return;}
    if(pend&&Math.hypot(e.clientX-pend.x,e.clientY-pend.y)>8){pend=null;pan={x:s.x,y:s.y};return;} /* on glisse avec un outil de pose : on déplace la vue */
    if(pan){ed.px-=(s.x-pan.x)/K();ed.pz-=(s.y-pan.y)/K();pan={x:s.x,y:s.y};lim();cmDraw();return;}
    if(!ed.drag||!ed.sel)return;const p=P(e),x=snap(p.x-ed.drag.dx),z=snap(p.z-ed.drag.dz);
    if(cmOk(ed,x,z,ed.sel)){ed.sel.x=x;ed.sel.z=z;cmSync(ed,ed.sel);cmDraw();}};
  cv.onpointerup=cv.onpointercancel=e=>{const had=pts.delete(e.pointerId);
    if(pend&&e.type==='pointerup'&&had&&!pinch&&pts.size===0){const p=P(e);pend=null;place(p);}
    pend=null;if(pts.size<2)pinch=null;if(pts.size===0){pan=null;ed.drag=null;}};
  $('cmZi').onclick=()=>zoomAt(cv.width/2,cv.height/2,1.4);
  $('cmZo').onclick=()=>zoomAt(cv.width/2,cv.height/2,1/1.4);
  $('cmZr').onclick=()=>{ed.zoom=1;ed.px=0;ed.pz=0;cmDraw();};
  $('cmPan').onclick=()=>{ed.panMode=!ed.panMode;$('cmPan').classList.toggle('on',ed.panMode);toast(ed.panMode?'Vue : un doigt déplace la vue':'Vue : un doigt pose / sélectionne');};
  $scr.querySelectorAll('[data-t]').forEach(b=>b.onclick=()=>{ed.tool=b.dataset.t;rMaker();});
  $scr.querySelectorAll('[data-p]').forEach(b=>b.onclick=()=>{const k=b.dataset.p;ed.sel=null;
    if(k==='_clr')ed.map.obj=[];else if(k==='_rnd')cmRandom(ed);else{ed.map.obj=[];CMP[k].f(ed);}rMaker();});
  $('cmName').onclick=()=>{const n=prompt('Nom de la carte',ed.map.name);if(n){ed.map.name=n.slice(0,24);rMaker();}};
  $('cmSz').onclick=()=>{ed.map.small=!ed.map.small;{const e=cmExt(ed.map);ed.map.obj=ed.map.obj.filter(o=>Math.abs(o.x)<(ed.map.small?40:96)+e&&Math.abs(o.z)<(ed.map.small?60:120)+e);}if(ed.map.seq)cmRenumber(ed.map);rMaker();};
  if($('cmSym'))$('cmSym').onclick=()=>{ed.map.sym=!ed.map.sym;rMaker();};
  if($('cmSeq'))$('cmSeq').onclick=()=>{const m=ed.map;m.seq=!m.seq;
    if(m.seq){m.defGoals=false;m.obj.forEach(o=>{delete o.tw;});cmSeqFix(m);toast('Séquence activée : les buts s\'ouvrent dans l\'ordre 1, 2, 3…');}
    ed.sel=null;rMaker();};
  $('cmDp').onclick=()=>{ed.map.defPads=ed.map.defPads===false;rMaker();};
  let txKey=null;
  $scr.querySelectorAll('[data-tx]').forEach(b=>b.onclick=()=>{txKey=b.dataset.tx;$('cmFile').click();});
  $scr.querySelectorAll('[data-txx]').forEach(b=>b.onclick=()=>{ed.map[b.dataset.txx]='';rMaker();});
  $('cmFile').onchange=e=>{const f=e.target.files&&e.target.files[0];if(!f||!txKey)return;
    if(f.size>12*1024*1024){toast('Image trop lourde (12 Mo max)');return;}
    imgPack(f,txKey==='tf'?512:256,u=>{if(!u){toast('Image illisible');return;}ed.map[txKey]=u;rMaker();});};
  if($('cmTwR'))$('cmTwR').onclick=()=>{const i=TWR.findIndex(q=>q[0]===ed.map.twr);ed.map.twr=TWR[(i+1)%TWR.length][0];rMaker();};
  $('cmMdS').onclick=()=>{cmLeavePark(ed);ed.map.mode='solo';if(ed.tool==='arrivee')ed.tool='sel';rMaker();};
  $('cmMdP').onclick=()=>{ed.map.mode='parkour';ed.map.seq=false;if(ed.tool==='but')ed.tool='sel';rMaker();};
  $('cmMdB').onclick=()=>{cmLeavePark(ed);ed.map.mode='bots';ed.map.seq=false;if(ed.tool==='arrivee')ed.tool='sel';rMaker();};
  if($('cmDg'))$('cmDg').onclick=()=>{ed.map.defGoals=ed.map.defGoals===false;rMaker();};
  if($('cmGs'))$('cmGs').onclick=()=>{const i=GSC.findIndex(q=>q[0]===ed.map.gs);ed.map.gs=GSC[(i+1)%GSC.length][0];rMaker();};
  $scr.querySelectorAll('[data-sz]').forEach(b=>b.onclick=()=>{ed.map.size=+b.dataset.sz;rMaker();});
  $('cmSave').onclick=()=>{const l=cmLoadAll(),c=JSON.parse(JSON.stringify(ed.map)),i=l.findIndex(m=>m&&m.name===c.name);if(i>=0)l.splice(i,1);l.push(c);if(l.length>12)l.shift();cmSaveAll(l);toast('Carte enregistrée : '+c.name);};
  $('cmOpen').onclick=()=>{const l=cmLoadAll();if(!l.length){toast('Aucune carte enregistrée');return;}
    modal('<div class="panel small"><h2>MES CARTES</h2>'+l.map((m,i)=>'<div class="row" style="display:flex;gap:6px;margin:6px 0"><button class="go alt" style="margin:0;flex:1" data-o="'+i+'">'+esc(m.name)+' ('+m.obj.length+')</button><button class="go alt" style="margin:0;width:56px" data-d="'+i+'">🗑️</button></div>').join('')+'<button class="go alt" id="cmX">Fermer</button></div>');
    $('mbox').querySelectorAll('[data-o]').forEach(b=>b.onclick=()=>{ed.map=cmFrom(JSON.parse(JSON.stringify(l[+b.dataset.o])));ed.sel=null;modalClose();rMaker();});
    $('mbox').querySelectorAll('[data-d]').forEach(b=>b.onclick=()=>{l.splice(+b.dataset.d,1);cmSaveAll(l);modalClose();});
    $('cmX').onclick=modalClose;};
  $('cmExp').onclick=()=>{const s=JSON.stringify(ed.map);try{navigator.clipboard.writeText(s).then(()=>toast('Code copié !'),()=>prompt('Copie ce code',s));}catch(e){prompt('Copie ce code',s);}};
  $('cmImp').onclick=()=>{const s=prompt('Colle le code de la carte');if(!s)return;try{const m=JSON.parse(s);if(!Array.isArray(m.obj))throw 0;m.obj=m.obj.filter(o=>CMT[o.t]).slice(0,160);ed.map=cmFrom(m);ed.sel=null;rMaker();}catch(e){toast('Code invalide');}};
  const launch=k=>{
    if(ed.map.seq){if(!ed.map.obj.some(o=>o.t==='but')){toast('Séquence : pose au moins un but');return;}cmRenumber(ed.map);}
    const hasG=ed.map.defGoals!==false||ed.map.obj.some(o=>o.t==='but');
    if(k==='casual'&&!hasG){toast('Ajoute au moins un but (ou active les cages classiques)');return;}
    CM.pend=cmCompile(ed.map);setCfg(k);if(k==='free'){cfg.size=1;}else cfg.size=clamp(ed.map.size||2,1,3);cfg.party=[];cfg.partySk=[];startMatch();};
  $('cmPub').onclick=commPublish;$('cmPriv').onclick=privPublish;
  if($('cmTest'))$('cmTest').onclick=()=>launch('free');$('cmPlay').onclick=()=>launch(cmAlone(ed.map)?'free':'casual');
}
function cmScale(cv){const [hx,hz]=cmDims(),e=cmExt(ED.map);return Math.min(cv.width/(2*(hx+e)+8),cv.height/(2*(hz+e)+8));}
function cmDraw(){
  const cv=$('cmC');if(!cv)return;const g=cv.getContext('2d'),[hx,hz]=cmDims(),k=cmScale(cv),ed=ED;
  g.setTransform(1,0,0,1,0,0);g.clearRect(0,0,cv.width,cv.height);g.translate(cv.width/2,cv.height/2);g.scale(k*ed.zoom,k*ed.zoom);g.translate(-ed.px,-ed.pz); /* vue du dessus : zoom + centre */
  const ex=cmExt(ed.map);
  if(ex){ /* parkour : le sol est entouré de vide, on peut y poser des éléments (zone pointillée) */
    g.fillStyle='#070d22';g.fillRect(-hx-ex,-hz-ex,2*(hx+ex),2*(hz+ex));
    g.strokeStyle='rgba(255,138,138,.35)';g.lineWidth=.6;g.setLineDash([4,4]);g.strokeRect(-hx-ex,-hz-ex,2*(hx+ex),2*(hz+ex));g.setLineDash([]);
    g.strokeStyle='rgba(120,170,255,.07)';g.lineWidth=.3;for(let v=-hx-ex+(ex%10);v<=hx+ex;v+=10){g.beginPath();g.moveTo(v,-hz-ex);g.lineTo(v,hz+ex);g.stroke();}for(let v=-hz-ex+(ex%10);v<=hz+ex;v+=10){g.beginPath();g.moveTo(-hx-ex,v);g.lineTo(hx+ex,v);g.stroke();}
    g.fillStyle='rgba(255,160,160,.45)';g.font='bold 7px Rajdhani,system-ui';g.textAlign='center';g.textBaseline='middle';g.fillText('VIDE — tu peux poser ici',0,-hz-ex/2);g.fillText('VIDE — tu peux poser ici',0,hz+ex/2);}
  g.fillStyle='#10244f';g.fillRect(-hx,-hz,2*hx,2*hz);
  {const fi=cmImg(ed.map.tf);if(fi){g.globalAlpha=.7;g.drawImage(fi,-hx,-hz,2*hx,2*hz);g.globalAlpha=1;}}
  g.strokeStyle='rgba(120,170,255,.12)';g.lineWidth=.3;for(let v=-hx;v<=hx;v+=10){g.beginPath();g.moveTo(v,-hz);g.lineTo(v,hz);g.stroke();}for(let v=-hz;v<=hz;v+=10){g.beginPath();g.moveTo(-hx,v);g.lineTo(hx,v);g.stroke();}
  if(ed.map.defGoals!==false&&ed.map.mode!=='parkour'){const k=ed.map.gs||1,gw=(ed.map.small?10:21)*k,gd=(ed.map.small?12:21)*Math.min(k,1.3);
    g.fillStyle='rgba(47,139,255,.35)';g.fillRect(-gw,-hz-gd,2*gw,gd);g.fillStyle='rgba(255,138,42,.35)';g.fillRect(-gw,hz,2*gw,gd);}
  g.strokeStyle='rgba(255,255,255,.5)';g.lineWidth=.6;g.strokeRect(-hx,-hz,2*hx,2*hz);g.beginPath();g.moveTo(-hx,0);g.lineTo(hx,0);g.stroke();g.beginPath();g.arc(0,0,ed.map.small?10:20,0,7);g.stroke();
  ed.map.obj.forEach(q=>{const t=CMT[q.t],sel=q===ed.sel;g.save();g.translate(q.x,q.z);g.rotate(-q.yaw*Math.PI/180);
    g.fillStyle=t.s==='pad'?(q.t==='bpad'?'#ffb347':'#ff9a30'):q.c;g.globalAlpha=t.s==='tor'?.15:(q.e>4?.65:.9);g.strokeStyle=sel?'#fff':'rgba(0,0,0,.6)';g.lineWidth=sel?1.2:.5;
    if(t.s==='goal'){ /* but : coque en U + ligne d'ouverture en pointillés + flèche vers l'extérieur (côté ouvert = bas quand rotation 0°) */
      const col=cmGoalCol(q,ed.map.mode),w2=q.w/2,d2=q.d/2,sh=shOf(q);
      if(sh==='ring'){g.globalAlpha=1;g.strokeStyle=sel?'#fff':col;g.lineWidth=sel?3:2.4;g.lineCap='round';g.beginPath();g.moveTo(-w2,0);g.lineTo(w2,0);g.stroke();g.lineCap='butt';g.fillStyle=sel?'#fff':col;g.beginPath();g.moveTo(0,8);g.lineTo(-4,2);g.lineTo(4,2);g.closePath();g.fill();}
      else if(sh!=='rect'){const N=SHN[sh],V=polyV(N,w2),ap=w2*Math.cos(Math.PI/N);
        g.beginPath();V.forEach((v,i)=>i?g.lineTo(v[0],v[1]):g.moveTo(v[0],v[1]));g.closePath();g.globalAlpha=.3;g.fillStyle=col;g.fill();g.globalAlpha=1;
        g.strokeStyle=sel?'#fff':col;g.lineWidth=sel?2.4:2;
        for(let k=0;k<N;k++){const a=V[(k+N-1)%N],b=V[k],op=polyOpen(sh,k,N);g.lineWidth=op?1:(sel?2.4:2);g.setLineDash(op?[2,2]:[]);g.beginPath();g.moveTo(a[0],a[1]);g.lineTo(b[0],b[1]);g.stroke();}
        g.setLineDash([]);g.fillStyle=sel?'#fff':col;g.beginPath();g.moveTo(0,ap+7);g.lineTo(-4,ap+1);g.lineTo(4,ap+1);g.closePath();g.fill();}
      else{
      g.globalAlpha=.3;g.fillStyle=col;g.fillRect(-w2,-d2,q.w,q.d);g.globalAlpha=1;
      g.strokeStyle=sel?'#fff':col;g.lineWidth=sel?2.4:2;g.beginPath();g.moveTo(-w2,d2);g.lineTo(-w2,-d2);g.lineTo(w2,-d2);g.lineTo(w2,d2);g.stroke();
      g.lineWidth=1;g.setLineDash([2,2]);g.beginPath();g.moveTo(-w2,d2);g.lineTo(w2,d2);g.stroke();g.setLineDash([]);
      g.fillStyle=sel?'#fff':col;g.beginPath();g.moveTo(0,d2+7);g.lineTo(-4,d2+1);g.lineTo(4,d2+1);g.closePath();g.fill();}}
    else if(t.s==='fin'&&shOf(q)!=='rect'){const V=polyV(SHN[shOf(q)],q.w/2);g.beginPath();V.forEach((v,i)=>i?g.lineTo(v[0],v[1]):g.moveTo(v[0],v[1]));g.closePath();g.globalAlpha=.5;g.fillStyle='#35ff9a';g.fill();g.globalAlpha=1;g.strokeStyle=sel?'#fff':'#bfffe0';g.lineWidth=sel?1.6:1;g.stroke();}
    else if(t.s==='fin'){g.globalAlpha=.5;g.fillStyle='#35ff9a';g.fillRect(-q.w/2,-q.d/2,q.w,q.d);g.globalAlpha=1;g.strokeStyle=sel?'#fff':'#bfffe0';g.lineWidth=sel?1.6:1;g.strokeRect(-q.w/2,-q.d/2,q.w,q.d);
      g.fillStyle='#fff';const n=Math.max(2,Math.round(q.w/4));for(let i=0;i<n;i++){if(i%2===0)g.fillRect(-q.w/2+i*q.w/n,-q.d/2,q.w/n,q.d/2);else g.fillRect(-q.w/2+i*q.w/n,0,q.w/n,q.d/2);}}
    else if(t.s==='box'){g.fillRect(-q.w/2,-q.d/2,q.w,q.d);g.strokeRect(-q.w/2,-q.d/2,q.w,q.d);if(q.pitch){g.beginPath();g.moveTo(0,-q.d/2);g.lineTo(0,q.d/2);g.stroke();}}
    else if(t.s==='pad'){g.beginPath();g.arc(0,0,q.w/2+1,0,7);g.fill();g.stroke();}
    else{g.beginPath();g.arc(0,0,q.w/2,0,7);g.fill();g.stroke();if(t.s==='tor'){g.lineWidth=Math.max(.8,q.d);g.strokeStyle=q.c;g.stroke();}}
    g.restore();});
  if(ed.map.mode==='solo'&&ed.map.seq){const L=cmGoalsSorted(ed.map);
    g.save();g.strokeStyle='rgba(53,255,154,.55)';g.lineWidth=.8;g.setLineDash([3,3]);g.beginPath();L.forEach((q,i)=>i?g.lineTo(q.x,q.z):g.moveTo(q.x,q.z));g.stroke();g.setLineDash([]);
    g.font='bold 11px Rajdhani,system-ui';g.textAlign='center';g.textBaseline='middle';g.lineJoin='round';g.lineWidth=3;
    L.forEach((q,i)=>{const t=String(i+1);g.strokeStyle='#000';g.strokeText(t,q.x,q.z);g.fillStyle=q===ed.sel?'#ffd23f':'#fff';g.fillText(t,q.x,q.z);});g.restore();}
  const i=$('cmInfo');if(i)i.textContent=ed.map.obj.length+' éléments • '+(ed.tool==='sel'?'touche un élément pour le déplacer':'touche la carte pour placer : '+CMT[ed.tool].n)+' • zoom ×'+ed.zoom.toFixed(1)+' (molette / pincer, glisser dans le vide = bouger la vue)';
}
function cmProps(){
  const el=$('cmProp'),o=ED.sel;if(!el)return;
  if(!o||!ED.map.obj.includes(o)){el.innerHTML='<h4>ÉLÉMENT</h4><div class="row"><span style="opacity:.6">Rien de sélectionné</span></div>';return;}
  const t=CMT[o.t],ed=t.ed,rows=[['yaw','Rotation',15,0,345,'°',1]],isG=t.s==='goal',solo=ED.map.mode==='solo',seq=solo&&!!ED.map.seq;
  if(isG&&seq){if(o.bd===undefined)o.bd=40;if(o.by===undefined)o.by=0;if(o.hv===undefined)o.hv=0;if(!(o.n>0))o.n=cmNextN(ED.map);}
  if(isG){rows.push(['w','Largeur',2,8,80,''],['h','Hauteur',1,6,40,''],['d','Profondeur',1,6,40,''],['e','Altitude',1,0,40,'']);
    if(seq)rows.push(['bd','Balle : distance',2,10,90,''],['by','Balle : altitude',1,0,60,'']);}
  else{
  if(ed.includes('w'))rows.push(['w',(t.s==='box'||t.s==='fin')?'Largeur':'Taille',1,1,80,'']);
  if(ed.includes('h'))rows.push(['h','Hauteur',1,.5,60,'']);
  if(ed.includes('d'))rows.push(['d',t.s==='tor'?'Épaisseur':'Longueur',t.s==='tor'?.5:1,.5,80,'']);
  if(t.s!=='pad')rows.push(['e','Altitude',1,-10,60,'']);
  if(ed.includes('p'))rows.push(['pitch','Inclinaison',5,-60,90,'°']);}
  const hasSh=isG||t.s==='fin',sh0=hasSh?shOf(o):'rect';
  if(isG&&sh0==='ring'){const wr=rows.find(r=>r[0]==='w');wr[1]='Diamètre';wr[3]=SHMINW.ring;wr[4]=60;for(const k of ['h','d']){const i=rows.findIndex(r=>r[0]===k);if(i>=0)rows.splice(i,1);}}
  else if(hasSh){const wr=rows.find(r=>r[0]==='w');wr[3]=sh0==='rect'?(isG?8:1):SHMINW[sh0];if(sh0!=='rect'){wr[1]='Diamètre';const i=rows.findIndex(r=>r[0]==='d');if(i>=0)rows.splice(i,1);}}
  el.innerHTML='<h4>ÉLÉMENT : '+t.n.toUpperCase()+'</h4>'+(isG?'<h4>TYPE DE BUT</h4><div class="pal" style="grid-template-columns:repeat(2,1fr)"><button class="t'+(sh0!=='ring'?' on':'')+'" data-gk="3d" style="font-size:11px">🥅 3D · Cage</button><button class="t'+(sh0==='ring'?' on':'')+'" data-gk="2d" style="font-size:11px">⭕ 2D · Cerceau</button></div>':'')+(hasSh&&sh0!=='ring'?'<h4>FORME</h4><div class="pal" style="grid-template-columns:repeat(4,1fr)">'+GSH.map(q=>'<button class="t'+(sh0===q[0]?' on':'')+'" data-sh="'+q[0]+'" style="font-size:11px">'+q[1]+'</button>').join('')+'</div>':'')+rows.map(r=>'<div class="row"><span>'+r[1]+'</span><button class="t" data-s="'+r[0]+'" data-v="-'+r[2]+'">−</button><b id="cv_'+r[0]+'">'+(+o[r[0]].toFixed(1))+r[5]+'</b><button class="t" data-s="'+r[0]+'" data-v="'+r[2]+'">+</button></div>').join('')+
   (isG?'<h4>TAILLE DU BUT</h4><div class="pal" style="grid-template-columns:repeat(5,1fr)">'+(sh0==='ring'?GRP.map((q,i)=>'<button class="t" data-gr="'+i+'" style="font-size:11px">'+q[0]+'</button>'):GPRE.map((q,i)=>'<button class="t" data-gp="'+i+'" style="font-size:11px">'+q[0]+'</button>')).join('')+'</div>'+
     (solo?'':'<div class="row"><span>Équipe du but</span>'+[0,1,2].map(n=>'<button class="t'+((o.team|0)===n?' on':'')+'" data-tm="'+n+'" style="width:auto;flex:1;font-size:12px">'+GTEAM[n]+'</button>').join('')+'</div><div class="row"><span style="opacity:.7;font-size:11px;font-weight:600">Bleu / Orange : défendu par cette équipe (point pour l\'autre). Neutre : point pour la dernière équipe qui a touché la balle.</span></div>')+
     '<div class="row"><span style="opacity:.7;font-size:11px;font-weight:600">'+(sh0==='ring'?(solo?'Passe la balle dans le cerceau (dans les deux sens) pour marquer.':'Passe la balle dans le cerceau en entrant par le côté de la flèche.'):'Côté ouvert = côté de la flèche.')+'</span></div>':'')+
   (isG&&seq?'<h4>SÉQUENCE</h4><div class="row"><span>Ordre dans la séquence</span><button class="t" data-ord="-1">−</button><b>'+o.n+' / '+cmGoalsSorted(ED.map).length+'</b><button class="t" data-ord="1">+</button></div>'+
     '<div class="row"><span>Balle flottante</span><button class="t'+(o.hv?' on':'')+'" id="cmHv" style="width:auto;padding:6px 10px">'+(o.hv?'OUI':'NON')+'</button></div>'+
     '<div class="row"><span style="opacity:.7;font-size:11px;font-weight:600">Balle : distance = écart devant l\'ouverture du but, altitude = hauteur où elle apparaît. Flottante : elle reste en l\'air jusqu\'à ce que tu la touches (utile pour les buts en hauteur).</span></div>':'')+
   (t.s!=='pad'&&!(isG&&(!solo||seq))?'<div class="row"><span>Couleur</span><button class="t" id="cmCol" style="width:60px;background:'+o.c+'">&nbsp;</button></div>':'')+
   '<div class="row"><button class="t" id="cmDup" style="flex:1;width:auto">⧉ Dupliquer</button><button class="t" id="cmDel" style="flex:1;width:auto">🗑️ Supprimer</button></div>';
  el.querySelectorAll('[data-s]').forEach(b=>b.onclick=()=>{const key=b.dataset.s,r=rows.find(x=>x[0]===key);let v=o[key]+ +b.dataset.v;
    v=key==='yaw'?(v+360)%360:clamp(v,r[3],r[4]);o[key]=v;cmSync(ED,o);$('cv_'+key).textContent=(+v.toFixed(1))+r[5];cmDraw();});
  el.querySelectorAll('[data-sh]').forEach(b=>b.onclick=()=>{o.sh=b.dataset.sh;if(o.sh!=='rect')o.w=Math.max(o.w,SHMINW[o.sh]);cmSync(ED,o);cmProps();cmDraw();});
  el.querySelectorAll('[data-gp]').forEach(b=>b.onclick=()=>{const q=GPRE[+b.dataset.gp];o.w=Math.max(q[1],SHMINW[shOf(o)]);o.h=q[2];o.d=q[3];cmSync(ED,o);cmProps();cmDraw();});
  el.querySelectorAll('[data-gk]').forEach(b=>b.onclick=()=>{
    if(b.dataset.gk==='2d'&&o.sh!=='ring'){o.sh='ring';o.w=24;o.h=o.d=24;o.e=Math.max(o.e,8);}
    else if(b.dataset.gk==='3d'&&o.sh==='ring'){o.sh='rect';o.w=42;o.h=15;o.d=21;o.e=0;}
    else return;
    cmSync(ED,o);cmProps();cmDraw();});
  el.querySelectorAll('[data-gr]').forEach(b=>b.onclick=()=>{const q=GRP[+b.dataset.gr];o.w=q[1];o.h=o.d=q[1];cmSync(ED,o);cmProps();cmDraw();});
  el.querySelectorAll('[data-tm]').forEach(b=>b.onclick=()=>{o.team=+b.dataset.tm;cmSync(ED,o);cmProps();cmDraw();});
  if($('cmCol'))$('cmCol').onclick=()=>{o.c=CMCOL[(CMCOL.indexOf(o.c)+1)%CMCOL.length];ED.col=CMCOL.indexOf(o.c);cmSync(ED,o);cmProps();cmDraw();};
  el.querySelectorAll('[data-ord]').forEach(b=>b.onclick=()=>{cmRenumber(ED.map);const L=cmGoalsSorted(ED.map),i=L.indexOf(o),j=i+ +b.dataset.ord;if(j<0||j>=L.length)return;const a=L[i].n;L[i].n=L[j].n;L[j].n=a;cmProps();cmDraw();});
  if($('cmHv'))$('cmHv').onclick=()=>{o.hv=o.hv?0:1;if(o.hv&&!o.by)o.by=Math.max(2,Math.round(o.e*.7));cmProps();cmDraw();};
  $('cmDup').onclick=()=>{const n=cmAdd(ED,o.t,o.x+4,o.z+4,{e:o.e,w:o.w,h:o.h,d:o.d,yaw:o.yaw,pitch:o.pitch,c:o.c,team:o.team,sh:o.sh,bd:o.bd,by:o.by,hv:o.hv});if(n){ED.sel=n;cmProps();cmDraw();}};
  $('cmDel').onclick=()=>{ED.map.obj=ED.map.obj.filter(q=>q!==o&&q.id!==o.tw);if(ED.map.seq)cmRenumber(ED.map);ED.sel=null;cmProps();cmDraw();};
}

/* =====================================================================
   CARTES DE LA COMMUNAUTÉ : Firestore, collection « maps »
   ===================================================================== */
const COMM={col:'maps',sort:'likes',cache:{},LK:'rocketArena3D.mapLikes',RP:'rocketArena3D.mapReports'};
function cmClean(o,lim){ /* données venues d'Internet : tout est borné et validé (lim : portée x/z, plus grande en parkour) */
  if(!o||!CMT[o.t])return null;
  const n=(v,a,b,d)=>{v=+v;return isFinite(v)?clamp(v,a,b):d;};
  lim=lim||CMLIM;const r={t:o.t,x:n(o.x,-lim,lim,0),z:n(o.z,-lim,lim,0),e:n(o.e,-10,60,0),w:n(o.w,.5,80,4),h:n(o.h,.5,60,4),d:n(o.d,.5,80,4),yaw:n(o.yaw,0,360,0),pitch:n(o.pitch,-60,90,0),c:/^#[0-9a-f]{6}$/i.test(o.c)?o.c:CMCOL[0],team:[0,1,2].indexOf(+o.team)>=0?+o.team:0};
  if(o.t==='but'){r.w=Math.max(8,r.w);r.h=clamp(r.h,6,40);r.d=clamp(r.d,6,40);r.e=clamp(r.e,0,40);r.pitch=0;
    r.n=n(o.n,0,99,0);r.bd=n(o.bd,10,90,40);r.by=n(o.by,0,60,0);r.hv=+o.hv?1:0;} /* séquence : ordre, distance / altitude de la balle, balle flottante */
  if(o.t==='but'||o.t==='arrivee'){r.sh=SHN[o.sh]!==undefined?o.sh:'rect';if(r.sh==='ring'&&o.t!=='but')r.sh='rect';if(r.sh!=='rect'){r.w=Math.max(r.w,SHMINW[r.sh]);r.d=r.w;}if(r.sh==='ring'){r.w=clamp(r.w,SHMINW.ring,60);r.h=r.d=r.w;}}
  return r;
}
function commEnc(m){const mm=cmFrom(m);return JSON.stringify([['_m',mm.mode,mm.defGoals?1:0,mm.gs,mm.size,mm.seq?1:0]].concat(m.obj.slice(0,160).map(o=>[o.t,o.x,o.z,o.e,o.w,o.h,o.d,o.yaw,o.pitch,o.c,o.team|0,o.sh||'rect',o.n|0,o.bd,o.by,o.hv?1:0])));}
function commMeta(s){let a;try{a=JSON.parse(s);}catch(e){a=null;}
  const r=Array.isArray(a)&&Array.isArray(a[0])&&a[0][0]==='_m'?a[0]:null;
  return cmFrom(r?{mode:r[1],defGoals:r[2]!==0,gs:r[3],size:r[4],seq:r[5]===1}:{});}
function commDec(s){let a;try{a=JSON.parse(s);}catch(e){return [];}if(!Array.isArray(a))return [];
  const lim=Array.isArray(a[0])&&a[0][0]==='_m'&&a[0][1]==='parkour'?CMLIM_PK:CMLIM;
  return a.slice(0,161).map(r=>Array.isArray(r)&&r[0]!=='_m'?cmClean({t:r[0],x:r[1],z:r[2],e:r[3],w:r[4],h:r[5],d:r[6],yaw:r[7],pitch:r[8],c:r[9],team:r[10],sh:r[11],n:r[12],bd:r[13],by:r[14],hv:r[15]},lim):null).filter(Boolean).slice(0,160);}
/* carte « déjà publiée » : le Parcours de buts, affiché en tête de la liste comme n'importe quelle carte de la communauté (likes / parties comptés en local) */
const COMM_BI='rocketArena3D.commBuiltin';
function commBIget(){try{return JSON.parse(localStorage.getItem(COMM_BI))||{};}catch(e){return {};}}
function commBIrow(){const st=commBIget(),m=grMap();return {id:'builtin:goalrun',builtin:1,name:m.name,author:'Rocket Arena',n:m.obj.length,small:false,dp:false,objs:commEnc(m),likes:+st.likes||0,plays:+st.plays||0,rep:0};}
function commSet(k){try{return JSON.parse(localStorage.getItem(k))||[];}catch(e){return [];}}
function commMark(k,id){const l=commSet(k);if(l.indexOf(id)<0){l.push(id);try{localStorage.setItem(k,JSON.stringify(l.slice(-300)));}catch(e){}}}
/* auteur d'une carte : les cartes officielles (compte staff) sont toujours signées « Rocket Arena » ; un joueur normal ne peut pas prendre ce nom */
const OFF_AUTHOR='Rocket Arena';
function commAuthor(r){return (r.builtin||r.off===true)?OFF_AUTHOR:String(r.author||'?');}
function commMyAuthor(){if(BAN.isStaff())return OFF_AUTHOR;const n=String(SV.name||'Joueur').slice(0,12);return n.replace(/\s+/g,'').toLowerCase()==='rocketarena'?'Joueur':n;}
async function commPublish(){
  const F=window.RA_FB,m=ED.map;
  if(!F){toast('Mode en ligne indisponible');return;}
  if(m.obj.length<3){toast('Ajoute au moins 3 éléments');return;}
  if(!m.name||m.name.trim().length<3||m.name==='Ma carte'){toast('Donne un vrai nom à ta carte (bouton Nom)');return;}
  let last=0;try{last=+localStorage.getItem('rocketArena3D.mapPub')||0;}catch(e){}
  if(Date.now()-last<60000){toast('Attends une minute entre deux publications');return;}
  if(m.pubId&&!confirm('Cette carte est déjà publiée. Publier une copie ?'))return;
  if(await BAN.mine()){toast('Tu es banni : publication impossible');return;}
  try{
    const ref=F.doc(F.collection(F.db,COMM.col));
    await F.setDoc(ref,Object.assign({name:m.name.trim().slice(0,24),author:commMyAuthor(),aid:LB.getId(),small:!!m.small,dp:m.defPads!==false,n:m.obj.length,objs:commEnc(m),likes:0,plays:0,rep:0,ts:F.serverTimestamp()},(BAN.isStaff()?{off:true}:{}),(m.tw&&m.mode!=='parkour'?{tw:texClean(m.tw),twr:m.twr}:{}),(m.tf?{tf:texClean(m.tf)}:{})));
    m.pubId=ref.id;try{localStorage.setItem('rocketArena3D.mapPub',String(Date.now()));}catch(e){}
    COMM.cache={};toast(BAN.isStaff()?'🌍 Carte officielle publiée !':'🌍 Carte publiée !');
  }catch(e){console.warn('Publication',e);toast('Publication impossible (règles Firestore ?)');}
}
/* =====================================================================
   CARTES PRIVÉES : publiées avec un code (collection Firestore « privmaps », id du document = code).
   Elles n'apparaissent jamais dans la liste publique : il faut connaître le code pour les jouer.
   Règles Firestore à ajouter : voir firestore-privmaps.rules.txt (lecture par code seulement, pas de liste).
   ===================================================================== */
const PRIV={col:'privmaps',KEY:'rocketArena3D.privCodes',AL:'ABCDEFGHJKMNPQRSTUVWXYZ23456789',tries:[]};
function privNorm(s){return String(s||'').toUpperCase().replace(/[^A-Z0-9]/g,'');}
function privFmt(c){return c.slice(0,4)+'-'+c.slice(4);}
function privGen(){const a=new Uint32Array(8);try{crypto.getRandomValues(a);}catch(e){for(let i=0;i<8;i++)a[i]=Math.floor(Math.random()*4294967296);}let c='';for(let i=0;i<8;i++)c+=PRIV.AL[a[i]%PRIV.AL.length];return c;}
function privList(){try{const l=JSON.parse(localStorage.getItem(PRIV.KEY));return Array.isArray(l)?l:[];}catch(e){return [];}}
function privRemember(c,n){const l=privList().filter(q=>q.c!==c);l.unshift({c:c,n:String(n).slice(0,24),t:Date.now()});try{localStorage.setItem(PRIV.KEY,JSON.stringify(l.slice(0,30)));}catch(e){}}
function privCopy(c){const t=privFmt(c);try{navigator.clipboard.writeText(t).then(()=>toast('Code copié : '+t),()=>prompt('Copie ce code',t));}catch(e){prompt('Copie ce code',t);}}
function privShow(code,name){
  modal('<div class="panel small"><h2>🔒 CARTE PRIVÉE</h2><div class="sub">'+esc(name)+'</div>'+
    '<div style="font:900 36px Rajdhani,system-ui;letter-spacing:5px;margin:12px 0;-webkit-user-select:text;user-select:text">'+privFmt(code)+'</div>'+
    '<div class="sub">Donne ce code à tes amis : dans CARTES DE LA COMMUNAUTÉ, ils tapent le code puis appuient sur 🔑. La carte n\'apparaît pas dans la liste publique.</div>'+
    '<button class="go" id="pvCp">📋 Copier le code</button><button class="go alt" id="pvX">Fermer</button></div>');
  $('pvCp').onclick=()=>privCopy(code);$('pvX').onclick=modalClose;}
function privMine(){
  const l=privList();if(!l.length){toast('Tu n\'as pas encore de carte privée');return;}
  modal('<div class="panel small"><h2>🔒 MES CODES</h2>'+l.map((q,i)=>'<div style="display:flex;gap:6px;align-items:center;margin:6px 0"><div style="flex:1;text-align:left;min-width:0"><b>'+esc(q.n)+'</b><br><small style="opacity:.8;letter-spacing:2px">'+privFmt(q.c)+'</small></div><button class="go alt" style="margin:0;width:56px" data-pc="'+i+'">📋</button></div>').join('')+'<button class="go alt" id="pvX">Fermer</button></div>');
  $('mbox').querySelectorAll('[data-pc]').forEach(b=>b.onclick=()=>privCopy(l[+b.dataset.pc].c));$('pvX').onclick=modalClose;}
async function privPublish(){
  const F=window.RA_FB,m=ED.map;
  if(!F){toast('Mode en ligne indisponible');return;}
  if(m.obj.length<3){toast('Ajoute au moins 3 éléments');return;}
  if(!m.name||m.name.trim().length<3||m.name==='Ma carte'){toast('Donne un vrai nom à ta carte (bouton Nom)');return;}
  let last=0;try{last=+localStorage.getItem('rocketArena3D.privPub')||0;}catch(e){}
  if(Date.now()-last<15000){toast('Attends quelques secondes entre deux cartes privées');return;}
  if(await BAN.mine()){toast('Tu es banni : publication impossible');return;}
  try{
    let code=null;
    for(let i=0;i<6&&!code;i++){const c=privGen();if(!(await F.getDoc(F.doc(F.db,PRIV.col,c))).exists())code=c;}
    if(!code){toast('Impossible de créer un code, réessaie');return;}
    await F.setDoc(F.doc(F.db,PRIV.col,code),Object.assign({name:m.name.trim().slice(0,24),author:commMyAuthor(),aid:LB.getId(),small:!!m.small,dp:m.defPads!==false,n:m.obj.length,objs:commEnc(m),ts:F.serverTimestamp()},(m.tw&&m.mode!=='parkour'?{tw:texClean(m.tw),twr:m.twr}:{}),(m.tf?{tf:texClean(m.tf)}:{})));
    try{localStorage.setItem('rocketArena3D.privPub',String(Date.now()));}catch(e){}
    privRemember(code,m.name.trim());privShow(code,m.name.trim());
  }catch(e){console.warn('Carte privée',e);toast('Publication impossible : ajoute les règles Firestore des cartes privées');}
}
async function privPlay(raw){
  const F=window.RA_FB,code=privNorm(raw);
  if(!F){toast('Mode en ligne indisponible');return;}
  if(code.length!==8){toast('Code invalide : 8 caractères (ex. K7M2-QX4P)');return;}
  const now=Date.now();PRIV.tries=PRIV.tries.filter(t=>now-t<60000);
  if(PRIV.tries.length>=6){toast('Trop d\'essais, attends une minute');return;}
  let d=null;
  try{const sn=await F.getDoc(F.doc(F.db,PRIV.col,code));if(sn.exists())d=sn.data();}
  catch(e){console.warn('Code privé',e);toast('Accès refusé : règles Firestore des cartes privées manquantes');return;}
  if(!d||typeof d.objs!=='string'){PRIV.tries.push(now);toast('Code inconnu');return;}
  const bn=await BAN.list();if(bn&&bn[d.aid]){toast('Cette carte n\'est plus disponible');return;}
  commPlay(Object.assign({},d,{id:'priv:'+code,priv:1}));
}
async function commFetch(sort,force){
  const F=window.RA_FB;if(!F)throw new Error('firebase');
  const c=COMM.cache[sort];if(!force&&c&&Date.now()-c.t<20000)return c.rows;
  const sn=await F.getDocs(F.query(F.collection(F.db,COMM.col),F.orderBy(sort,'desc'),F.limit(40)));
  const bn=await BAN.list();
  const rows=sn.docs.map(d=>Object.assign({id:d.id},d.data())).filter(r=>typeof r.objs==='string'&&(+r.rep||0)<5&&!bn[r.aid]);
  COMM.cache[sort]={t:Date.now(),rows:rows};return rows;
}
async function commBump(id,f){
  if(String(id).indexOf('priv:')===0)return; /* carte privée : pas de compteurs */
  if(String(id).indexOf('builtin:')===0){const st=commBIget();st[f]=(+st[f]||0)+1;try{localStorage.setItem(COMM_BI,JSON.stringify(st));}catch(e){}return;}
  const F=window.RA_FB;if(!F)return;
  try{const ref=F.doc(F.db,COMM.col,id);
    await F.runTransaction(F.db,async t=>{const s=await t.get(ref);if(!s.exists())return;t.update(ref,{[f]:(+s.data()[f]||0)+1});});
  }catch(e){console.warn('Carte : mise à jour impossible',e);}
}
function cmThumb(cv,small,obj,mode,seq){
  const g=cv.getContext('2d'),hx=small?40:96,hz=small?60:120,k=Math.min(cv.width/(2*hx+4),cv.height/(2*hz+4));
  g.setTransform(1,0,0,1,0,0);g.clearRect(0,0,cv.width,cv.height);g.translate(cv.width/2,cv.height/2);g.scale(k,k);
  g.fillStyle='#10244f';g.fillRect(-hx,-hz,2*hx,2*hz);g.strokeStyle='rgba(255,255,255,.5)';g.lineWidth=1.2;g.strokeRect(-hx,-hz,2*hx,2*hz);
  g.save();if(mode==='parkour'){g.beginPath();g.rect(-hx,-hz,2*hx,2*hz);g.clip();} /* parkour : ce qui est dans le vide reste secret */
  obj.forEach(q=>{const t=CMT[q.t];g.save();g.translate(q.x,q.z);g.rotate(-q.yaw*Math.PI/180);g.fillStyle=t.s==='pad'?'#ff9a30':(t.s==='goal'?(mode==='solo'?q.c:GCOL[q.team|0]):(t.s==='fin'?'#35ff9a':q.c));g.globalAlpha=.9;
    if((t.s==='goal'||t.s==='fin')&&shOf(q)!=='rect'){g.beginPath();g.arc(0,0,q.w/2,0,7);g.fill();}
    else if(t.s==='box'||t.s==='goal'||t.s==='fin')g.fillRect(-q.w/2,-q.d/2,q.w,q.d);else{g.beginPath();g.arc(0,0,t.s==='pad'?3:q.w/2,0,7);if(t.s==='tor'){g.lineWidth=Math.max(2,q.d);g.strokeStyle=q.c;g.stroke();}else g.fill();}
    g.restore();});
  if(seq){const L=obj.filter(q=>q.t==='but').map((q,i)=>[q,i]).sort((a,b)=>((a[0].n||0)-(b[0].n||0))||(a[1]-b[1])).map(q=>q[0]);
    g.strokeStyle='rgba(53,255,154,.6)';g.lineWidth=1.2;g.setLineDash([4,4]);g.beginPath();L.forEach((q,i)=>i?g.lineTo(q.x,q.z):g.moveTo(q.x,q.z));g.stroke();g.setLineDash([]);
    g.font='bold 15px Rajdhani,system-ui';g.textAlign='center';g.textBaseline='middle';g.lineWidth=3;g.lineJoin='round';
    L.forEach((q,i)=>{g.strokeStyle='#000';g.strokeText(i+1,q.x,q.z);g.fillStyle='#fff';g.fillText(i+1,q.x,q.z);});}
  g.restore();
}
function commPlay(r){
  const m=Object.assign(commMeta(r.objs),{name:r.name,small:!!r.small,defPads:r.dp!==false,obj:commDec(r.objs),tw:texClean(r.tw),tf:texClean(r.tf),twr:r.twr});
  if(!m.obj.length){toast('Carte vide ou invalide');return;}
  if(!partyOK(['comm']))return;
  const solo=cmAlone(m);
  CM.pend=cmCompile(m);setCfg(solo?'free':'casual');
  if(solo){cfg.size=1;cfg.party=[];cfg.partySk=[];}else{cfg.size=clamp(m.size||2,1,3);partyCfg(cfg.size);}
  commBump(r.id,'plays');startMatch();
}
function rComm(){
  const S=COMM.sort,tab=(k,t)=>'<button class="opt'+(S===k?' on':'')+'" data-cs="'+k+'">'+t+'</button>';
  $scr.innerHTML='<div class="cpanel wide"><h2>CARTES DE LA COMMUNAUTÉ</h2><div class="sub">Cartes créées par les joueurs. Fabrique la tienne dans CRÉATEUR, puis publie-la.</div>'+
    '<div class="opts" style="margin:8px 0;align-items:center"><input id="pvIn" maxlength="9" placeholder="CODE PRIVÉ" autocomplete="off" autocapitalize="characters" spellcheck="false" style="width:150px;padding:9px 10px;font:800 16px Rajdhani,system-ui;letter-spacing:3px;text-transform:uppercase;text-align:center;background:rgba(255,255,255,.1);color:#fff;border:1px solid rgba(255,255,255,.3);border-radius:8px;-webkit-user-select:text;user-select:text"><button class="opt" id="pvGo">🔑 Jouer</button><button class="opt" id="pvMy">🔒 Mes codes</button></div>'+
    '<div class="opts" style="margin:8px 0">'+tab('likes','❤ Populaires')+tab('ts','🕒 Récentes')+tab('plays','▶ Les plus jouées')+'<button class="opt" data-cs="_r">↻</button>'+(BAN.isStaff()?'<button class="opt" id="banPanel">🛡️ Bannis</button>':'')+'</div>'+
    '<div id="commL" class="lbmsg">Chargement…</div><button class="go alt" data-go="lobby">Retour</button></div>';
  $scr.querySelectorAll('[data-cs]').forEach(b=>b.onclick=()=>{const k=b.dataset.cs;if(k==='_r')COMM.cache={};else COMM.sort=k;rComm();});
  if($('banPanel'))$('banPanel').onclick=()=>BAN.panel();
  $('pvGo').onclick=()=>privPlay($('pvIn').value);$('pvMy').onclick=privMine;$('pvIn').onkeydown=e=>{if(e.key==='Enter')privPlay($('pvIn').value);};
  commFetch(S).then(r0=>r0,e=>{console.warn(e);return null;}).then(rows0=>{
    const box=$('commL');if(!box||UI.scr!=='comm')return;
    const rl=rows0||[],stf=BAN.isStaff();
    const rows=[commBIrow()].concat(rl.filter(r=>r.off===true),rl.filter(r=>r.off!==true)); /* cartes officielles (staff) toujours en tête */
    const liked=commSet(COMM.LK),rep=commSet(COMM.RP);
    box.className='';box.innerHTML=rows.map((r,i)=>'<div style="display:flex;gap:10px;align-items:center;margin:6px 0;padding:8px;background:rgba(255,255,255,.07)"><canvas width="60" height="75" data-th="'+i+'" style="background:#0a1330;flex:none"></canvas>'+
      '<div style="flex:1;min-width:0;text-align:left"><b style="font-size:18px">'+esc(r.name||'?')+'</b><br><small style="opacity:.75">par '+esc(commAuthor(r))+' • '+(+r.n||0)+' éléments • '+(r.small?'petite':'standard')+' • '+(commMeta(r.objs).seq?'séquence de buts':({solo:'solo',parkour:'parkour',bots:'avec bots'})[commMeta(r.objs).mode])+((r.builtin||r.off===true)?' • ★ officielle':'')+' • ❤ <span data-lk="'+i+'">'+(+r.likes||0)+'</span> • ▶ '+(+r.plays||0)+'</small></div>'+
      '<button class="opt" data-l="'+i+'"'+(liked.indexOf(r.id)>=0?' disabled':'')+'>❤</button>'+(r.builtin?'':'<button class="opt" data-f="'+i+'"'+(rep.indexOf(r.id)>=0?' disabled':'')+' title="Signaler">🚩</button>')+(stf&&!r.builtin?'<button class="opt" data-bn="'+i+'" title="Bannir l\'auteur">🔨</button><button class="opt" data-dl="'+i+'" title="Supprimer la carte">🗑️</button>':'')+
      '<button class="go" data-pl="'+i+'" style="width:auto;margin:0;padding:8px 16px;font-size:18px">JOUER</button></div>').join('');
    box.querySelectorAll('canvas[data-th]').forEach(c=>{const r=rows[+c.dataset.th];cmThumb(c,!!r.small,commDec(r.objs),commMeta(r.objs).mode,commMeta(r.objs).seq);});
    box.querySelectorAll('[data-pl]').forEach(b=>b.onclick=()=>commPlay(rows[+b.dataset.pl]));
    box.querySelectorAll('[data-bn]').forEach(b=>b.onclick=async()=>{const r=rows[+b.dataset.bn];if(!r.aid||!confirm('Bannir '+(r.off===true?'l\'auteur de cette carte':(r.author||'cet auteur'))+' ?'))return;if(await BAN.ban(r.aid,r.author))rComm();});
    box.querySelectorAll('[data-dl]').forEach(b=>b.onclick=async()=>{const r=rows[+b.dataset.dl],F=window.RA_FB;if(!F||!confirm('Supprimer la carte « '+r.name+' » ?'))return;
      try{await F.deleteDoc(F.doc(F.db,COMM.col,r.id));COMM.cache={};toast('Carte supprimée');rComm();}catch(e){console.warn(e);toast('Suppression impossible (règles Firestore ?)');}});
    box.querySelectorAll('[data-l]').forEach(b=>b.onclick=()=>{const r=rows[+b.dataset.l];b.disabled=true;commMark(COMM.LK,r.id);r.likes=(+r.likes||0)+1;box.querySelector('[data-lk="'+b.dataset.l+'"]').textContent=r.likes;commBump(r.id,'likes');});
    box.querySelectorAll('[data-f]').forEach(b=>b.onclick=()=>{const r=rows[+b.dataset.f];if(!confirm('Signaler cette carte ?'))return;b.disabled=true;commMark(COMM.RP,r.id);commBump(r.id,'rep');toast('Merci, carte signalée');});
    if(rows0===null)box.insertAdjacentHTML('beforeend','<div class="lbmsg">Cartes en ligne indisponibles pour l\'instant (connexion ou règles Firestore).</div>');
  }).catch(e=>{const box=$('commL');if(box)box.textContent='Impossible de charger les cartes (connexion ou règles Firestore).';console.warn(e);});
}

/* ---------- CASIER ---------- */
function rLocker(){
  const tabs=CATS.map(c=>'<button class="tab'+(UI.cat===c.k?' on':'')+'" data-tab="'+c.k+'">'+c.ic+' '+c.n+'</button>').join('')+'<button class="tab'+(UI.cat==='color'?' on':'')+'" data-tab="color">🌈 Couleurs</button>';
  let body='';
  if(UI.cat==='color'){
    const sw=(key)=>'<div class="grp"><b>'+(key==='c1'?'COULEUR PRINCIPALE (modes sans équipe)':'COULEUR D\'ACCENT (roues, décalques, toppers)')+'</b><div class="sw">'+COLORS.map((c,i)=>'<button class="swb'+(SV.load[key]===i?' on':'')+'" data-col="'+key+'" data-v="'+i+'" style="background:'+c[1]+'" title="'+c[0]+'"></button>').join('')+'</div></div>';
    body=sw('c1')+sw('c2')+'<div class="help">En match d\'équipe, la carrosserie prend la couleur de ton équipe ; la couleur d\'accent s\'applique toujours.</div>';
  }else{
    let list=LST[UI.cat];if(UI.ownL)list=list.filter(i=>owns(i.id));
    const tot=LST[UI.cat].length,have=LST[UI.cat].filter(i=>owns(i.id)).length;
    body='<div class="sub">'+have+' / '+tot+' possédés <button class="opt'+(UI.ownL?' on':'')+'" data-a="own" style="margin-left:8px;flex:none;padding:4px 10px">Possédés</button></div><div class="grid">'+list.map(cardHTML).join('')+'</div>';
  }
  const pre=[0,1,2,3].map(i=>'<button class="opt'+(SV.preset===i?' on':'')+'" data-pre="'+i+'">'+(i+1)+(SV.presets[i]?'':'·')+'</button>').join('');
  $scr.innerHTML='<div class="rpanel"><div class="tabs">'+tabs+'</div><div class="pbody">'+body+'</div><div class="pfoot" id="lfoot"></div></div>';
  hydrate($scr);lockerFoot(pre);
}
function lockerFoot(pre){
  pre=pre||[0,1,2,3].map(i=>'<button class="opt'+(SV.preset===i?' on':'')+'" data-pre="'+i+'">'+(i+1)+(SV.presets[i]?'':'·')+'</button>').join('');
  const it=UI.sel?IT[UI.sel]:null;let info='';
  if(it){
    const own=owns(it.id),r=RAR[it.r];
    info='<div class="finfo"><b style="color:'+r.c+'">'+esc(it.n)+'</b> <small>'+r.n+(it.sub?' • '+it.sub:'')+'</small></div>'+
      (own?(SV.load[it.cat]===it.i?'<span class="okk">✔ Équipé</span>':'<button class="go sm" data-a="equip">Équiper</button>'):
      '<button class="go sm" data-a="buy">Acheter 💰 '+it.price.toLocaleString('fr-FR')+'</button>');
  }else info='<div class="finfo"><small>Touche un objet pour l\'équiper ou l\'essayer.</small></div>';
  const f=$('lfoot');if(!f)return;
  f.innerHTML='<div class="frow">'+info+'</div><div class="frow"><small>CASIER</small><div class="opts" style="flex:1">'+pre+'</div><button class="opt" data-a="savepre" style="flex:none;padding:8px 12px">💾 Enregistrer</button></div>';
}
function lockerClick(id){
  const it=IT[id];if(!it)return;
  if(owns(id)){equipItem(it);previewItem(it);UI.sel=id;}
  else previewItem(it);
  const pb=$scr.querySelector('.pbody');const st=pb?pb.scrollTop:0;
  if(UI.scr==='locker'){rLocker();const p2=$scr.querySelector('.pbody');if(p2)p2.scrollTop=st;}
  else lockerFoot();
}
/* ---------- BOUTIQUE ---------- */
function rShop(){
  UI.feat={};shopFeatured().forEach(i=>{UI.feat[i.id]=1;});
  const tabs=[['feat','⭐ À la une'],['cat','📚 Catalogue'],['crate','📦 Caisses'],['bund','🎁 Lots']].map(t=>'<button class="tab'+(UI.shop===t[0]?' on':'')+'" data-shop="'+t[0]+'">'+t[1]+'</button>').join('');
  let body='';
  if(UI.shop==='feat'){
    const left=86400000-(Date.now()%86400000),hh=Math.floor(left/3600000),mm=Math.floor(left/60000)%60;
    body='<div class="sub">Offres du jour — renouvellement dans '+hh+' h '+mm+' min</div><h3>À LA UNE <small>−20 %</small></h3><div class="grid">'+shopFeatured().map(cardHTML).join('')+'</div><h3>OFFRES DU JOUR</h3><div class="grid">'+shopDaily().map(cardHTML).join('')+'</div>';
  }else if(UI.shop==='cat'){
    const chips=CATS.map(c=>'<button class="chip2'+(UI.cr===c.k?' on':'')+'" data-cr="'+c.k+'">'+c.ic+'</button>').join('');
    let list=LST[UI.cr].filter(i=>i.price>0);
    if(UI.cf>=0)list=list.filter(i=>i.r===UI.cf);if(UI.ownS)list=list.filter(i=>!owns(i.id));
    const rch=[[-1,'Toutes']].concat(RAR.map((r,i)=>[i,r.n])).map(r=>'<button class="chip2'+(UI.cf===r[0]?' on':'')+'" data-cf="'+r[0]+'">'+r[1]+'</button>').join('');
    body='<div class="chips">'+chips+'</div><div class="chips">'+rch+'<button class="chip2'+(UI.ownS?' on':'')+'" data-a="own">Non possédés</button></div><div class="grid">'+list.map(cardHTML).join('')+'</div>';
  }else if(UI.shop==='crate'){
    body='<div class="sub">Ouvre une caisse pour gagner un objet aléatoire. Les doublons sont remboursés à 35 %.</div><div class="crates">'+CRATES.map((c,i)=>'<div class="crate" style="--rc:'+c.c+'"><div class="cbox">📦</div><b>'+c.n+'</b><small>'+['Surtout des objets communs et rares','Rares à exotiques','Import, exotique et marché noir'][i]+'</small><button class="go sm" data-crate="'+i+'">💰 '+c.price+'</button></div>').join('')+'</div>';
  }else{
    body='<div class="sub">Lots à −28 % : prix calculé sur les objets que tu ne possèdes pas encore.</div>'+BUNDLES.map((b,i)=>{
      const p=bundlePrice(b);
      return '<div class="bund"><div><b>'+esc(b.n)+'</b><div class="bi">'+b.ids.map(id=>'<span class="bchip" style="border-color:'+RAR[IT[id].r].c+'">'+CATS.find(c=>c.k===IT[id].cat).ic+' '+esc(IT[id].n)+(owns(id)?' ✔':'')+'</span>').join('')+'</div></div><button class="go sm" data-bund="'+i+'"'+(p<=0?' disabled':'')+'>'+(p<=0?'Possédé':'💰 '+p.toLocaleString('fr-FR'))+'</button></div>';}).join('');
  }
  $scr.innerHTML='<div class="rpanel"><div class="tabs">'+tabs+'</div><div class="pbody">'+body+'</div><div class="pfoot" id="lfoot"></div></div>';
  hydrate($scr);shopFoot();
}
function shopFoot(){
  const f=$('lfoot');if(!f)return;
  const it=UI.sel?IT[UI.sel]:null;
  if(!it){f.innerHTML='<div class="finfo"><small>Touche un objet pour le voir sur ta voiture.</small></div>';return;}
  const own=owns(it.id),r=RAR[it.r],p=priceOf(it);
  f.innerHTML='<div class="frow"><div class="finfo"><b style="color:'+r.c+'">'+esc(it.n)+'</b> <small>'+r.n+(it.sub?' • '+it.sub:'')+'</small></div>'+
    (own?'<span class="okk">✔ Possédé</span><button class="go sm" data-a="equip">Équiper</button>':'<button class="go sm" data-a="buy">Acheter 💰 '+p.toLocaleString('fr-FR')+'</button>')+'</div>';
}
function doBuy(){
  const it=UI.sel?IT[UI.sel]:null;if(!it||owns(it.id))return;
  const p=UI.scr==='shop'?priceOf(it):it.price;
  if(SV.credits<p){toast('Crédits insuffisants');return;}
  SV.credits-=p;SV.owned[it.id]=1;saveSV();toast('Acheté : '+it.n);SFX.pad(true);refreshTop();
  const st=($scr.querySelector('.pbody')||{}).scrollTop||0;
  if(UI.scr==='shop'){rShop();}else rLocker();
  const pb=$scr.querySelector('.pbody');if(pb)pb.scrollTop=st;
}
function crateReveal(i){
  const r=openCrate(i);if(!r){toast('Crédits insuffisants');return;}
  refreshTop();initAudio();
  const col=RAR[r.it.r].c;
  modal('<h2>CAISSE OUVERTE !</h2><div class="reveal" style="--rc:'+col+'"><img id="rimg" class="cv" alt=""><b style="color:'+col+'">'+esc(r.it.n)+'</b><small>'+RAR[r.it.r].n+' • '+CATS.find(c=>c.k===r.it.cat).n+'</small></div>'+
    (r.dup?'<div class="sub">Doublon : +'+r.refund+' crédits</div>':'<div class="sub" style="color:#7dffb0">Ajouté à ton casier !</div>')+'<button class="go" data-a="mclose">OK</button>');
  thumbReq(r.it,$('rimg'));
  tone(520,1040,.3,'sine',.12);tone(780,1560,.4,'sine',.1,.15);
  if(UI.scr==='shop')rShop();
}
/* ---------- PASSE DE SAISON ---------- */
function rPass(){
  const tier=passTier(),nxt=(SV.xp%PASS_XP)/PASS_XP*100;
  $scr.innerHTML='<div class="cpanel"><h2>PASSE DE SAISON</h2><div class="sub">Gagne de l\'XP en jouant : un palier tous les '+PASS_XP+' XP. Les récompenses sont débloquées automatiquement.</div>'+
    '<div class="bar big"><i style="width:'+(tier>=40?100:nxt)+'%"></i></div><div class="sub">Palier '+tier+' / 40 • '+SV.xp+' XP</div><div class="grid pass">'+
    PASS.map(r=>{
      const done=r.t<=SV.pass.claimed,it=r.type==='item'?IT[r.val]:null;
      return '<div class="pcard'+(done?' done':'')+'" '+(it?'style="--rc:'+RAR[it.r].c+'"':'')+'><small>'+r.t+'</small>'+(it?'<b>'+CATS.find(c=>c.k===it.cat).ic+'</b><span>'+esc(it.n)+'</span>':'<b>💰</b><span>'+r.val+'</span>')+(done?'<em>✔</em>':'')+'</div>';
    }).join('')+'</div></div>';
}
/* ---------- DÉFIS + STATS ---------- */
function rChal(){
  chSync();
  const tab=UI.ctab||'d',c2=ch2Sync(),nd=ndSync(),sn=seasonNow();
  const TB=[['d','Du jour'],['n','New Driver'],['w','Hebdo'],['s','Saison'],['e','Événement']];
  let h='<div class="cpanel"><h2>DÉFIS</h2><div class="tabs" style="padding:0 0 8px">'+TB.map(t=>'<button class="tab'+(tab===t[0]?' on':'')+'" data-ctab="'+t[0]+'">'+t[1]+(t[0]==='n'&&!nd.done?' ❗':'')+'</button>').join('')+'</div>';
  if(tab==='n'){
    h+='<div class="sub">'+(nd.done?'✔ Compétitif débloqué'+(nd.start?' • rang de départ : '+esc(rankOf(nd.start).n):''):'Termine ces défis pour débloquer le Compétitif. Ta performance fixe ton rang de départ.')+'</div>'+
      ND_DEFS.map(c=>chRow(c,nd.done?c.v:(nd.p[c.k]||0),false,false)).join('');
  }else if(tab==='w'||tab==='s'||tab==='e'){
    const L={w:chWeek,s:chSeason,e:chEvent}[tab](),st=c2[tab];
    const info={w:'Se renouvellent dans '+leftTxt(weekLeftMs()),s:'Saison '+sn.n+' • se termine dans '+leftTxt(sn.end-Date.now()),e:'🎉 '+evNow().t+' • événement limité, se termine dans '+leftTxt(weekLeftMs())}[tab];
    h+='<div class="sub">'+info+'</div>'+L.map(c=>chRow(c,st.p[c.k]||0,st.c[c.id],true)).join('');
  }else{
    h+=chToday().map(c=>chRow(c,SV.ch.prog[c.id]||0,SV.ch.claimed[c.id],true)).join('')+
    '<h3>CLASSEMENT</h3><div class="ranks">'+[1,2,3].map(s=>{const mm=mmrOf(''+s),rk=rankInfo(s);return '<div class="rank" style="border-color:'+rk.c+'"><span style="color:'+rk.c+'">'+rk.n.toUpperCase()+'</span><small>'+SZN[s]+' • '+(rk.un?(PLACE_N-rk.left)+' / '+PLACE_N+' placement':mm+' MMR')+'</small></div>';}).join('')+'</div>'+
    '<h3>STATISTIQUES</h3><div class="stats">'+[['Matchs',SV.stats.matches],['Victoires',SV.stats.wins],['Buts',SV.stats.goals],['Passes',SV.stats.assists],['Arrêts',SV.stats.saves],['Tirs',SV.stats.shots],['Démos',SV.stats.demos],['Caisses',SV.crates||0]].map(s=>'<div><b>'+s[1]+'</b><small>'+s[0]+'</small></div>').join('')+'</div>';
  }
  $scr.innerHTML=h+'</div>';
}
/* ---------- RÉGLAGES ---------- */
function rSet(){
  const s=SV.set;
  $scr.innerHTML='<div class="cpanel"><h2>RÉGLAGES</h2>'+
    '<div class="grp"><b>PSEUDO</b><input id="inName" maxlength="12" value="'+esc(SV.name)+'" class="txt"></div>'+
    '<div class="grp"><b>VOLUME</b><input id="inVol" type="range" min="0" max="100" value="'+Math.round(s.vol*100)+'" style="width:100%"></div>'+
    '<div class="grp"><b>VOLUME DE LA MUSIQUE DU MENU</b><input id="inMVol" type="range" min="0" max="100" value="'+Math.round((s.mvol==null?.7:s.mvol)*100)+'" style="width:100%"></div>'+
    '<div class="grp"><b>ÉCRAN DE VEILLE</b><select id="inSaver" class="txt">'+[[0,'Désactivé'],[30,'Après 30 secondes'],[60,'Après 1 minute'],[120,'Après 2 minutes'],[300,'Après 5 minutes']].map(o=>'<option value="'+o[0]+'"'+(saverDelay()===o[0]?' selected':'')+'>'+o[1]+'</option>').join('')+'</select></div>'+
    '<div class="grp"><b>CHAMP DE VISION <span id="camFV"></span></b><input id="camF" type="range" min="60" max="110" step="1" style="width:100%"></div>'+
    '<div class="grp"><b>DISTANCE CAMÉRA <span id="camDV"></span></b><input id="camD" type="range" min="7" max="18" step="0.5" style="width:100%"></div>'+
    '<div class="grp"><b>HAUTEUR CAMÉRA <span id="camHV"></span></b><input id="camH" type="range" min="2" max="9" step="0.2" style="width:100%"></div>'+
    '<div class="grp"><b>RIGIDITÉ CAMÉRA <span id="camSV"></span></b><input id="camS" type="range" min="4" max="20" step="1" style="width:100%"></div>'+
    grp('ACCÉLÉRATION TACTILE','autoThr',[[1,'Automatique'],[0,'Manuelle']],s.autoThr)+
    '<div class="grp"><b>TEXTURE DES MURS</b><div style="display:flex;gap:10px;align-items:center;flex-wrap:wrap;margin-top:6px">'+
      '<div id="wallPrev" style="width:64px;height:64px;border:2px solid rgba(255,255,255,.3);background:#0a1330 center/cover no-repeat'+(WALL.url?';background-image:url('+WALL.url+')':'')+'"></div>'+
      '<button class="opt" id="wallPick" style="flex:none;padding:10px 16px">📁 Choisir une image</button><button class="opt" id="wallDef" style="flex:none;padding:10px 16px">↺ Par défaut</button>'+
      '<input id="wallFile" type="file" accept="image/*" style="display:none"></div>'+
      '<div class="opts" style="margin-top:6px">'+[[2,'Motif petit'],[1,'Motif normal'],[.35,'Motif grand']].map(v=>'<button class="opt'+(WALL.rep===v[0]?' on':'')+'" data-wr="'+v[0]+'">'+v[1]+'</button>').join('')+'</div></div>'+
    '<div class="help">Choisis une image de ton appareil : elle remplace la texture des murs de l\'arène au prochain match (cartes avec murs). Le motif se répète sur les murs.</div>'+
    grp('TOUT DÉBLOQUER (mode créatif)','unlock',[[0,'Non'],[1,'Oui']],s.unlockAll?1:0)+
    '<div class="help">Le mode créatif débloque toutes les voitures et tous les objets dans le casier, sans les acheter.</div>'+
    '<div class="grp"><b>DONNÉES</b><button class="opt" data-a="reset" style="flex:none;padding:10px 16px">Réinitialiser la sauvegarde</button></div>'+
    '<div class="help">Commandes : WASD/ZQSD/flèches • Espace saut (2× = flip) • Maj/clic gauche boost • Ctrl/X dérapage • C caméra • Tab scores • P pause • F ou clic droit : power-up (Rumble) • R : remettre la balle (entraînement)</div></div>';
  $('wallPick').onclick=()=>$('wallFile').click();
  $('wallFile').onchange=e=>{const f=e.target.files&&e.target.files[0];if(!f)return;
    if(f.size>12*1024*1024){toast('Image trop lourde (12 Mo max)');return;}
    const r=new FileReader();r.onload=()=>wallFromUrl(r.result,false,(ok,err)=>{if(err){toast('Image illisible');return;}toast(ok?'Texture des murs enregistrée':'Appliquée, mais trop lourde pour être gardée');if(UI.scr==='settings'||$('wallPrev'))rSet();});
    r.onerror=()=>toast('Image illisible');r.readAsDataURL(f);};
  $('wallDef').onclick=()=>{wallReset();toast('Texture des murs par défaut');rSet();};
  $scr.querySelectorAll('[data-wr]').forEach(b=>b.onclick=()=>{WALL.rep=+b.dataset.wr;wallApplyRep();if(WALL.url)wallSave();rSet();});
  $('inName').oninput=e=>{SV.name=e.target.value.slice(0,12);saveSV();refreshTop();};
  $('inVol').oninput=e=>{SV.set.vol=e.target.value/100;saveSV();applyVolume();};
  $('inSaver').onchange=e=>{SV.set.saver=+e.target.value;saveSV();SAVER.last=performance.now();};
  $('inMVol').oninput=e=>{SV.set.mvol=e.target.value/100;saveSV();musApplyVol();};
  [['camF',78],['camD',11.5],['camH',4.4],['camS',10]].forEach(([k,d])=>{const i=$(k);if(!i)return;i.value=SV.set[k]||d;$(k+'V').textContent=i.value;
    i.oninput=e=>{SV.set[k]=+e.target.value;$(k+'V').textContent=e.target.value;saveSV();};});
}
/* ---------- TOURNOI ---------- */
function tourPairs(r){
  const t=SV.tour;const part=r===0?[0,1,2,3,4,5,6,7]:t.res[r-1];const out=[];
  for(let i=0;i<part.length;i+=2)out.push([part[i],part[i+1]]);return out;
}
function rTour(){
  const t=SV.tour;if(!t){tourNew();return rTour();}
  let h='<div class="cpanel wide"><h2>TOURNOI</h2><div class="sub">'+(t.champ?'🏆 Tu as gagné le tournoi !':(t.alive?TOUR_ROUNDS[t.round]+' — à toi de jouer.':'Tu es éliminé.'))+'</div><div class="bracket">';
  for(let r=0;r<3;r++){
    h+='<div class="bcol"><small>'+TOUR_ROUNDS[r]+'</small>';
    if(r<=t.round||t.res[r-1]){
      tourPairs(r).forEach((p,i)=>{
        const w=t.res[r]&&t.res[r][i];
        h+='<div class="bm">'+p.map(ti=>'<div class="bt'+(ti===0?' me':'')+(w===ti?' win':'')+((w!==undefined&&w!==ti)?' out':'')+'">'+esc(t.teams[ti])+'</div>').join('')+'</div>';
      });
    }else h+='<div class="bm"><div class="bt">?</div><div class="bt">?</div></div>';
    h+='</div>';
  }
  h+='</div>'+(t.alive&&!t.champ?'<button class="go" data-a="tourPlay">JOUER LE MATCH ▶</button>':'<button class="go" data-a="tourNew">NOUVEAU TOURNOI</button>')+'<button class="go alt" data-go="lobby">Retour</button></div>';
  $scr.innerHTML=h;
}
function tourFinish(win){
  const t=SV.tour;if(!t)return;
  const pairs=tourPairs(t.round),rg=mulberry((Date.now()/100)|0);
  t.res[t.round]=pairs.map(p=>{
    if(p.indexOf(0)>=0)return win?0:(p[0]===0?p[1]:p[0]);
    return rg()<.5?p[0]:p[1];
  });
  if(win){
    SV.credits+=TOUR_PRIZE[t.round];
    if(t.round===2){t.champ=true;t.alive=false;const it=shopPick((Date.now()/1000)|0,1,3,5,SV.owned)[0];if(it)SV.owned[it.id]=1;}
    else t.round++;
  }else t.alive=false;
  saveSV();
}
/* ---------- actions ---------- */
function applyVolume(){if(master)master.gain.value=SV.set.vol;}
const ACT={
  play:doPlay,
  own:()=>{if(UI.scr==='shop'){UI.ownS=!UI.ownS;rShop();}else{UI.ownL=!UI.ownL;rLocker();}},
  equip:()=>{const it=UI.sel&&IT[UI.sel];if(it&&owns(it.id)){equipItem(it);(UI.scr==='shop'?shopFoot:lockerFoot)();if(UI.scr==='locker')rLocker();}},
  buy:doBuy,mclose:modalClose,
  savepre:()=>{SV.presets[SV.preset]=Object.assign({},SV.load);saveSV();toast('Casier '+(SV.preset+1)+' enregistré');rLocker();},
  reset:()=>{if(confirm('Effacer toute la progression ?'+(CLOUD.user?' (la sauvegarde de ton compte sera aussi remplacée)':''))){try{localStorage.removeItem(SAVE_KEY);}catch(e){}SV=loadSV();saveSV();lobby.L=SV.load;go('lobby');toast('Sauvegarde réinitialisée');}},
  tourPlay:()=>{const t=SV.tour;cfg.mode='tour';cfg.size=2;cfg.mut=null;cfg.lg=false;partyCfg(2);cfg.diff=Math.min(2,t.round+(SV.menu.diff>1?1:0));cfg.dur=3;cfg.team=SV.menu.team;cfg.skill=.5+t.round*.18;startQueue(()=>startMatch());},
  tourNew:()=>{tourNew();rTour();},
  lgNew:()=>{lgNew();rSeason();},
  lgPlay:()=>{const g=SV.lg;if(!g||g.done)return;if(PARTY.length>1){toast('Mode saison : 2 joueurs maximum par équipe');return;}cfg.mode='casual';cfg.size=2;cfg.diff=Math.min(2,Math.floor(g.i/3));cfg.dur=3;cfg.team=SV.menu.team;cfg.skill=.42+g.i*.09;cfg.mut=null;cfg.lg=true;partyCfg(2);startMatch();}
};
$('ui').addEventListener('click',e=>{
  initAudio();
  const t=e.target.closest('[data-acc],[data-lb],[data-go],[data-card],[data-mode],[data-opt],[data-mut],[data-a],[data-id],[data-tab],[data-pre],[data-col],[data-shop],[data-cr],[data-cf],[data-crate],[data-bund],[data-ch],[data-ctab],[data-multi]');
  if(!t)return;const d=t.dataset;
  if(d.go==='quit'){quitGame();return;}
  if(d.lb){if(d.lb==='refresh')UI.lbForce=true;else UI.lb=d.lb;rLb();return;}
  if(d.acc){CLOUD.act(d.acc);return;}
  if(d.go){UI.home=false;go(d.go);return;}
  if(d.card){const c=CARDS.find(x=>x.k===d.card);if(c&&c.m.indexOf(SV.menu.mode)<0){SV.menu.mode=c.m[0];SV.menu.multi=[];saveSV();}rLobby();return;}
  if(d.mode){SV.menu.mode=d.mode;SV.menu.multi=[];saveSV();rLobby();return;}
  if(d.opt){
    const v=d.v;let nv=isNaN(+v)?v:+v;
    if(d.opt==='ballCam'){SV.set.ballCam=nv;cfg.ballCam=nv;}
    else if(d.opt==='autoThr'){SV.set.autoThr=nv;cfg.autoThr=nv;}
    else if(d.opt==='unlock'){SV.set.unlockAll=nv===1;}
    else SV.menu[d.opt]=nv;
    saveSV();
    if(UI.scr==='set'){rSet();refreshTop();}else rOpts();
    return;}
  if(d.mut){SV.menu.mut=SV.menu.mut||{};const v=d.v;SV.menu.mut[d.mut]=isNaN(+v)?v:+v;saveSV();rOpts();return;}
  if(d.a){if(ACT[d.a])ACT[d.a]();return;}
  if(d.id){if(UI.scr==='shop'){previewItem(IT[d.id]);shopFoot();document.querySelectorAll('.card.sel').forEach(c=>c.classList.remove('sel'));t.classList.add('sel');}else lockerClick(d.id);return;}
  if(d.tab){UI.cat=d.tab;UI.sel=null;lobbyFocus(d.tab==='color'?'car':d.tab);lobbyRefresh(SV.load);rLocker();return;}
  if(d.pre!==undefined){const i=+d.pre;SV.preset=i;if(SV.presets[i]){Object.assign(SV.load,SV.presets[i]);toast('Casier '+(i+1)+' chargé');}else{SV.presets[i]=Object.assign({},SV.load);toast('Casier '+(i+1)+' enregistré');}saveSV();lobby.L=SV.load;lobbyRefresh(SV.load);refreshTop();rLocker();return;}
  if(d.col){SV.load[d.col]=+d.v;saveSV();lobbyRefresh(SV.load);rLocker();return;}
  if(d.shop){UI.shop=d.shop;UI.sel=null;rShop();return;}
  if(d.cr){UI.cr=d.cr;rShop();return;}
  if(d.cf!==undefined){UI.cf=+d.cf;rShop();return;}
  if(d.crate!==undefined){crateReveal(+d.crate);return;}
  if(d.bund!==undefined){const b=BUNDLES[+d.bund];if(buyBundle(b)){toast('Lot acheté !');refreshTop();rShop();}else toast('Crédits insuffisants');return;}
  if(d.ctab){UI.ctab=d.ctab;rChal();return;}
  if(d.multi){toggleMulti(d.multi);return;}
  if(d.ch&&d.ch.charAt(1)===':'){chClaim(d.ch);return;}
  if(d.ch){const c=chToday().find(x=>x.id===d.ch);if(c&&(SV.ch.prog[c.id]||0)>=c.v&&!SV.ch.claimed[c.id]){SV.ch.claimed[c.id]=1;SV.credits+=c.cr;SV.xp+=c.xp;passSync();saveSV();toast('+'+c.cr+' crédits');refreshTop();rChal();}return;}
});
$('modal').addEventListener('click',e=>{const t=e.target.closest('[data-a]');if(t&&t.dataset.a==='mclose')modalClose();});
document.addEventListener('change',e=>{if(e.target&&e.target.dataset&&e.target.dataset.a==='unlock')return;});
$('daily').addEventListener('click',()=>{
  const r=claimDaily();
  if(r){toast('🎁 Récompense du jour : +'+r+' crédits (série '+SV.daily.streak+')');refreshTop();}else toast('Récompense déjà récupérée aujourd\'hui');
});


/* =====================================================================
   PHYSIQUE
   ===================================================================== */
function alignUp(c,target,rate){
  c.up.lerp(target,rate).normalize();
  tA.copy(c.fwd).addScaledVector(c.up,-c.fwd.dot(c.up));
  if(tA.lengthSq()<1e-4)tA.copy(c.right).cross(c.up);
  tA.normalize();c.fwd.copy(tA);c.right.crossVectors(c.fwd,c.up);
  mtx.makeBasis(c.right,c.up,tB.copy(c.fwd).negate());c.q.setFromRotationMatrix(mtx);
}
function dodge(c){
  const I=c.in;let f=-I.y,r=I.x;const m=Math.hypot(f,r);f/=m;r/=m;c.flipF=f;c.flipR=r;c.dodgeAge=0;
  tC.set(0,0,0).addScaledVector(c.fwd,f).addScaledVector(c.right,r);
  c.vel.addScaledVector(tC,PH.dodge);c.vel.addScaledVector(c.up,-c.vel.dot(c.up)*.45);
  c.flipAxis.crossVectors(c.up,tC).normalize();c.flipT=.5;c.flipRate=Math.PI*2/.5;c.hitBoost=.55;
}
function stepCar(c,dt){
  if(c.ghost)return;
  if(c.dead){c.deadT-=dt;if(c.deadT<=0)placeCar(c);return;}
  c.dodgeAge+=dt;
  powerStep(c,dt);
  const I=c.in;
  let ix=I.x,iy=I.y,ith=I.thr,iroll=I.roll;
  if(c.scr>0){ix=-ix;iy=-iy;ith=-ith;iroll=-iroll;}
  c.fwd.set(0,0,-1).applyQuaternion(c.q);c.up.set(0,1,0).applyQuaternion(c.q);c.right.set(1,0,0).applyQuaternion(c.q);
  c.noGround=Math.max(0,c.noGround-dt);c.jumpT=Math.max(0,c.jumpT-dt);c.flipClock-=dt;c.flipT=Math.max(0,c.flipT-dt);c.hitBoost=Math.max(0,c.hitBoost-dt);
  const wasG=c.grounded;let justJumped=false;
  c.boosting=false;
  if(I.boost&&MUT.boost!=='none'&&(c.boost>0||MUT.boost==='inf')){
    c.boosting=true;
    if(MUT.boost!=='inf')c.boost=Math.max(0,c.boost-33.3*dt);
  }
  if(MUT.boost==='rech'&&!c.boosting)c.boost=Math.min(100,c.boost+9*dt);
  if(MUT.boost==='inf')c.boost=100;
  if(I.jumpEdge){
    I.jumpEdge=false;
    if(c.grounded){c.vel.addScaledVector(c.up,PH.jump);c.jumpT=.2;c.jumpUp.copy(c.up);c.noGround=.12;c.grounded=false;c.flipAvail=true;c.flipClock=1.5;justJumped=true;if(c===player)SFX.jump();}
    else if(c.flipAvail&&c.flipClock>0){c.flipAvail=false;if(Math.hypot(ix,iy)>.4){const sx=I.x,sy=I.y;I.x=ix;I.y=iy;dodge(c);I.x=sx;I.y=sy;}else c.vel.addScaledVector(c.up,PH.flip);}
  }
  if(c.jumpT>0&&I.jump)c.vel.addScaledVector(c.jumpUp,PH.hold*dt);
  c.vel.y-=G*dt;
  if(c.grounded){
    const fs=c.vel.dot(c.fwd),ls=c.vel.dot(c.right),afs=Math.abs(fs);
    const th=ith;
    if(Math.abs(th)>.05){
      if(Math.sign(th)===Math.sign(fs)||afs<1){c.vel.addScaledVector(c.fwd,th*Math.max(0,34*(1-afs/34))*dt);}
      else c.vel.addScaledVector(c.fwd,th*58*dt);
    }else c.vel.addScaledVector(c.fwd,-Math.sign(fs)*Math.min(afs,7*dt));
    if(afs>34&&!c.boosting&&Math.abs(th)>.05&&Math.sign(th)===Math.sign(fs))c.vel.addScaledVector(c.fwd,-Math.sign(fs)*10*dt);
    if(I.drift)c.vel.addScaledVector(c.fwd,-Math.sign(fs)*Math.min(afs,13*dt));
    const turn=lerp(2.5,.95,clamp(afs/40,0,1))*(I.drift?1.65:1)*Math.min(1,.3+afs/5);
    qa.setFromAxisAngle(c.up,-ix*turn*dt);c.q.premultiply(qa).normalize();
    c.fwd.set(0,0,-1).applyQuaternion(c.q);c.right.set(1,0,0).applyQuaternion(c.q);
    c.vel.addScaledVector(c.right,-ls*(1-Math.exp(-(I.drift?2.2:20)*dt)));
    c.vel.addScaledVector(c.gn,-PH.stick*dt);
  }else{
    if(c.flipT>0){
      // annulation de flip : pousse la direction opposée à la flip
      if(c.flipF!==undefined&&(-iy*c.flipF+ix*c.flipR)<-.5){c.flipRate*=Math.exp(-30*dt);if(c.flipRate<1.5){c.flipT=0;c.flipRate=0;}}
      qa.setFromAxisAngle(c.flipAxis,c.flipRate*dt);c.q.premultiply(qa).normalize();c.av.multiplyScalar(.9);
    }else{
      let roll=iroll,yr=-ix*4;
      if(I.rollMode){roll+=ix;yr=0;}
      const noIn=Math.abs(ix)<.1&&Math.abs(iy)<.1&&Math.abs(iroll)<.1;
      if(noIn&&c.up.y<.3){roll+=clamp(c.right.y*3,-1,1)||(c.up.y<0?1:0);}
      tD.set(0,0,0).addScaledVector(c.right,iy*5.2).addScaledVector(c.up,yr).addScaledVector(c.fwd,roll*6);
      c.av.lerp(tD,dampT(12,dt));
      const ang=c.av.length()*dt;
      if(ang>1e-5){qa.setFromAxisAngle(tE.copy(c.av).normalize(),ang);c.q.premultiply(qa).normalize();}
    }
  }
  if(c.boosting)c.vel.addScaledVector(c.fwd,PH.boost*dt);
  const sp=c.vel.length();if(sp>54)c.vel.multiplyScalar(54/sp);
  c.pos.addScaledVector(c.vel,dt);

  // collisions avec l'arène (3 sphères le long de la voiture)
  let nc=0;tF.set(0,0,0);
  for(let k=-1;k<=1;k++){
    const o=k*1.3;
    sdf(c.pos.x+c.fwd.x*o,c.pos.y+c.fwd.y*o,c.pos.z+c.fwd.z*o,S,false);
    const dist=-S.d;
    if(dist<CR+.12){
      const nx=-S.nx,ny=-S.ny,nz=-S.nz,pen=CR-dist;
      if(pen>0){
        c.pos.x+=nx*pen;c.pos.y+=ny*pen;c.pos.z+=nz*pen;
        const vn=c.vel.x*nx+c.vel.y*ny+c.vel.z*nz;
        if(vn<0){const kk=vn<-10?1.25:1;c.vel.x-=nx*vn*kk;c.vel.y-=ny*vn*kk;c.vel.z-=nz*vn*kk;}
      }
      tF.x+=nx;tF.y+=ny;tF.z+=nz;nc++;
    }
  }
  c.grounded=false;
  if(nc>0){
    tF.normalize();
    if(c.up.dot(tF)>.25&&c.noGround<=0){
      c.grounded=true;c.gn.copy(tF);c.stuckT=0;
      alignUp(c,tF,dampT(16,dt));
    }else if(c.noGround<=0&&tF.y>.7&&c.up.y<-.2){
      c.stuckT+=dt;
      if(c.stuckT>.9){c.stuckT=0;c.up.set(0,1,0);alignUp(c,tB.set(0,1,0),1);c.vel.y+=7;c.av.set(0,0,0);}
    }
  }else c.stuckT=0;
  if(!wasG&&c.grounded){if(MODE.moves)mvLand(c,c.flipT);c.flipAvail=false;c.flipClock=0;c.flipT=0;c.av.set(0,0,0);}
  if(wasG&&!c.grounded&&!justJumped&&c.jumpT<=0){c.flipAvail=true;c.flipClock=1.5;}
  // sécurité
  {const LX=OPEN?400:200,LY=OPEN?220:100; /* parkour : des éléments peuvent être posés loin, dans le vide */
  if(!(c.pos.x===c.pos.x)||Math.abs(c.pos.x)>LX||Math.abs(c.pos.y)>LY||Math.abs(c.pos.z)>LX){placeCar(c);}}
}

function canDemo(a,dir,imp){
  if(a.demT>0&&a.fwd.dot(dir)>.3)return true;
  return a.boosting&&a.vel.length()>=40&&a.fwd.dot(dir)>.5&&imp>10;
}
function carCar(a,b){
  if(a.dead||b.dead||a.ghost||b.ghost)return;
  tA.copy(b.pos).sub(a.pos);const d=tA.length(),ds=a.hb.r+b.hb.r;
  if(d>ds||d<.01)return;
  tA.divideScalar(d);const pen=ds-d;
  a.pos.addScaledVector(tA,-pen*.5);b.pos.addScaledVector(tA,pen*.5);
  tB.copy(b.vel).sub(a.vel);const vn=tB.dot(tA);
  if(vn<0){
    let j=-(1.25)*vn*.5;
    if(a.hayT>0){j*=3;a.hayT=0;}
    if(b.hayT>0){j*=3;b.hayT=0;}
    a.vel.addScaledVector(tA,-j);b.vel.addScaledVector(tA,j);
    if(MUT.demo!=='off'&&(a.team!==b.team||MUT.demo==='all')){
      if(canDemo(a,tA,-vn))demolish(b,a);
      else{tD.copy(tA).negate();if(canDemo(b,tD,-vn))demolish(a,b);}
    }
  }
}
function demolish(v,by){
  if(ball.held===v)releaseBall(v);
  v.dead=true;v.deadT=3;v.mesh.visible=false;v.shadow.visible=false;
  burst(v.pos,0xffa030,70,24,.9);burst(v.pos,0xffffff,30,30,.6);
  by.stats.demos++;by.stats.score+=25;
  if(by===player)popup('DÉMOLITION',25,'#ff7a1a');
  if(v===player||by===player)addShake(1);
  feed(by.name+' a démoli '+v.name,TCSS[by.team]);
  if(v===player||by===player)SFX.demo();
}
function carBall(c){
  if(c.dead||c.ghost||c.noBall||!ball.alive||ball.held)return;
  tA.copy(ball.pos).sub(c.pos);
  const lx=tA.dot(c.right),ly=tA.dot(c.up),lz=tA.dot(c.fwd);
  const hb=c.hb,hx=hb.hx,hy=hb.hy,hz=hb.hz;
  const cx=clamp(lx,-hx,hx),cy=clamp(ly,-hy,hy),cz=clamp(lz,-hz,hz);
  const dx=lx-cx,dy=ly-cy,dz=lz-cz,d2=dx*dx+dy*dy+dz*dz;
  if(d2>=BR*BR)return;
  let dist=Math.sqrt(d2),nx,ny,nz;
  if(dist<1e-4){nx=0;ny=1;nz=0;dist=0;}else{nx=dx/dist;ny=dy/dist;nz=dz/dist;}
  tB.set(0,0,0).addScaledVector(c.right,nx).addScaledVector(c.up,ny).addScaledVector(c.fwd,nz);
  const pen=BR-dist;
  ball.pos.addScaledVector(tB,pen*.92);c.pos.addScaledVector(tB,-pen*.08);
  tC.copy(ball.vel).sub(c.vel);const vn=tC.dot(tB);
  if(ball.last!==c){
    ball.prev=ball.last;ball.prevT=ball.lastT;ball.last=c;
    ball.halo.material.color.set(TEAM[c.team]);
  }
  ball.lastT=clockT;
  // glaçon : la balle se fige sur place
  if(c.frzT>0&&ball.frozen<=0){
    c.frzT=0;ball.frozen=3.5;ball.vel.set(0,0,0);burst(ball.pos,0xbfe6ff,40,14,.8);SFX.hit(18);return;
  }
  if(ball.frozen>0){ball.frozen=0;burst(ball.pos,0xbfe6ff,30,14,.6);}
  // pointes : la balle reste collée à l'avant
  if(c.spkT>0&&nz>.4){
    c.spkT=0;ball.held=c;c.spkHold=3.5;ball.vel.copy(c.vel);SFX.hit(10);return;
  }
  if(vn<0&&!(c.pinT&&clockT-c.pinT<PINCH_FREE)){
    let j=-.8*vn;
    let mul=1+(c.hitBoost>0?.4:0)+(c.boosting?.08:0);
    if(c.phT>0){mul*=2.1;c.phT=0;burst(ball.pos,0xff5a2a,30,22,.5);}
    ball.vel.addScaledVector(tB,j*mul);
    /* impulsion Psyonix : direction centre voiture->balle (verticale x0.35), intensité selon la vitesse relative */
    const rv=-vn,psc=rv<11.7?.65:rv<54?lerp(.65,.55,(rv-11.7)/42.3):lerp(.55,.3,Math.min(1,(rv-54)/54));
    tD.copy(ball.pos).sub(c.pos);tD.y*=MODE.puck?.1:.35;tD.normalize();
    ball.vel.addScaledVector(tD,rv*psc*mul);
    c.vel.addScaledVector(tB,-j*.05);
    if(c.hayT>0){
      c.hayT=0;tD.copy(ball.pos).sub(c.pos).normalize();tD.y+=.12;tD.normalize();
      ball.vel.copy(tD).multiplyScalar(BMAX*.9);burst(ball.pos,0xffd166,40,28,.6);addShake(.5);
    }
    /* PINCH : la voiture écrase la balle contre le sol ou un mur -> elle est éjectée bien plus vite (une fois par contact) */
    if(rv>7&&clockT-(c.pinT||-9)>.35){
      sdf(ball.pos.x,ball.pos.y,ball.pos.z,S,true);
      if(S.d+BR>-.15&&tB.x*S.nx+tB.y*S.ny+tB.z*S.nz>.3){
        c.pinT=clockT;PINCH.t=clockT;PINCH.c=c;PINCH.cs=c.vel.length();PINCH.k=S.ny<-.6?'sol':'mur';
        const sn=tB.x*S.nx+tB.y*S.ny+tB.z*S.nz;tD.set(tB.x-S.nx*sn,tB.y-S.ny*sn,tB.z-S.nz*sn);
        const tl=tD.length();
        if(tl>.05){tD.divideScalar(tl);ball.vel.addScaledVector(tD,rv*mul);}
        ball.vel.x-=S.nx*rv*.12;ball.vel.y-=S.ny*rv*.12;ball.vel.z-=S.nz*rv*.12;
        burst(ball.pos,0xffd166,22,18,.5);if(c===player||c.coach)addShake(.3);
      }
    }
    const sp=ball.vel.length();if(sp>BMAX)ball.vel.multiplyScalar(BMAX/sp);
    if(-vn>3){
      SFX.hit(-vn);
      if(-vn>9)burst(ball.pos,TEAM[c.team],14,16,.45);
      if(c===player&&-vn>14)addShake(Math.min(.8,-vn/45));
      if(c===player&&-vn>26)popup('FRAPPE PUISSANTE',0,'#ffd166');
    }
    if(c===player&&-vn>6&&clockT-(c.medT||0)>2.5){
      const atk=c.team===0?1:-1;
      if(!c.grounded&&ball.pos.y>7){c.medT=clockT;c.stats.score+=20;popup('FRAPPE AÉRIENNE',20,'#6db0ff');}
      else if(ball.pos.z*atk<-HZ*.35&&ball.vel.z*atk>14){c.medT=clockT;c.stats.score+=20;popup('DÉGAGEMENT',20,'#7dffb0');}
      else if(ball.pos.z*atk>HZ*.3&&Math.abs(ball.pos.x)>14&&Math.abs(ball.vel.x)>10&&ball.vel.z*atk>0&&Math.abs(ball.pos.x+ball.vel.x*1.2)<GW+6){c.medT=clockT;c.stats.score+=10;popup('PASSE CENTRÉE',10,'#ffd166');}
    }
    /* FLIP RESET (règle du vrai jeu) : il faut que les 4 roues, donc le DESSOUS de la voiture, touchent la balle en l'air :
       normale de contact à moins d'environ 40° du « bas » de la voiture ET balle centrée sous le châssis (pas juste le bout du nez ou de l'arrière) */
    const wheels=ny<-.78&&Math.abs(lx)<hx*1.2&&Math.abs(lz)<hz*1.15;
    let rsGot=false;
    if(!c.grounded&&wheels&&-vn>.3){
      rsGot=flipReset(c);
      /* contact « doux » : les roues amortissent la balle, qui reste près de la voiture au lieu de partir comme un boulet (c'est ce qui laisse le temps de re-flipper dessus) */
      if(rsGot)ball.vel.lerp(c.vel,.88);
    }
    if(MODE.moves)mvTouch(c,nx,ny,nz,-vn,wheels,rsGot);
    hsTouch(c);
    dropTouch(c,-vn);
    if(HASGOAL)onTouch(c);
  }
}
/* FLIP RESET : les 4 roues touchent la balle en l'air -> le jeu croit que la voiture a « touché le sol » :
   - le flip (double saut) est rendu, et SANS limite de temps (le délai de 1,5 s ne compte que depuis un saut au sol) ;
   - ça ne change rien si le flip n'était pas encore dépensé : on prolonge juste le délai (comme dans Rocket League).
   Renvoie true seulement si un flip dépensé / expiré a vraiment été rendu. */
function flipReset(c){
  if(c.flipAvail&&c.flipClock>1e8)return false;               // déjà un flip « illimité » : rien à rendre
  const spent=!c.flipAvail||c.flipClock<=0;
  c.flipAvail=true;c.flipClock=1e9;
  if(!spent){burst(ball.pos,0x9fd0ff,8,8,.3);return false;}   // flip pas encore utilisé : seulement le délai prolongé
  c.resetT=clockT;c.resets=(c.resets||0)+1;
  burst(ball.pos,0xffd166,30,17,.6);burst(ball.pos,0xffffff,14,24,.35);
  if(c===player||c.coach){popup('FLIP RESET',0,'#ffd166');addShake(.35);SFX.reset();}
  return true;
}
/* pastille « flip dispo » : visible tant que le joueur garde un flip rendu par un reset */
let _rsChip=null;
function rsChipUpdate(){
  const p=player,on=!!(p&&!p.dead&&!p.grounded&&p.flipAvail&&p.flipClock>1e8&&phase==='play');
  if(!_rsChip){
    if(!on)return;
    _rsChip=document.createElement('div');
    _rsChip.style.cssText='position:fixed;left:50%;bottom:22%;transform:translateX(-50%);z-index:6;pointer-events:none;padding:6px 16px;border-radius:999px;font:900 15px/1 system-ui,sans-serif;letter-spacing:2px;color:#2a1c00;background:linear-gradient(90deg,#ffd166,#ffb347);box-shadow:0 0 22px rgba(255,200,80,.8);transition:opacity .15s';
    _rsChip.textContent='♻ FLIP DISPO';document.body.appendChild(_rsChip);
  }
  _rsChip.style.opacity=on?(.75+.25*Math.sin(clockT*14)):0;
}
function onTouch(c){
  const atk=c.team===0?1:-1;
  if(ball.shotTeam>=0&&ball.shotTeam!==c.team){c.stats.saves++;c.stats.score+=50;feed('Arrêt de '+c.name+' !',TCSS[c.team]);if(c===player)popup('ARRÊT',50,'#7dffb0');ball.shotTeam=-1;}
  const vz=ball.vel.z*atk;
  if(vz>14){
    const t=(atk*HZ-ball.pos.z)/(ball.vel.z||1e-3);
    if(t>0&&t<4){const xa=ball.pos.x+ball.vel.x*t;if(Math.abs(xa)<GW+5){if(ball.shotTeam!==c.team){c.stats.shots++;c.stats.score+=20;if(c===player)popup('TIR CADRÉ',20,'#6db0ff');}ball.shotTeam=c.team;}}
  }
}
function stepBall(dt){
  const b=ball;
  b.py=b.pos.y;
  if(b.frozen>0){
    b.frozen-=dt;b.vel.set(0,0,0);
    if(b.frozen<=0)burst(b.pos,0xbfe6ff,24,12,.6);
    return;
  }
  if(b.held){
    const c=b.held;
    if(c.dead){releaseBall(c);}
    else{
      b.pos.copy(c.pos).addScaledVector(c.fwd,c.hb.hz+BR*.7).addScaledVector(c.up,.35);
      /* la balle collée ne doit pas traverser le sol ni les murs */
      for(let it=0;it<3;it++){
        sdf(b.pos.x,b.pos.y,b.pos.z,S,true);
        const pen=S.d+BR;if(pen<=0)break;
        b.pos.x-=S.nx*pen;b.pos.y-=S.ny*pen;b.pos.z-=S.nz*pen;
      }
      b.vel.copy(c.vel);
      checkScore(b);return;
    }
  }
  hsSteer(dt);
  if(!(MODE.heat&&HS.team>=0))b.vel.y-=G*((MODE.moves&&MV.S&&MV.S.ceil&&MOVES[MV.i]&&MOVES[MV.i].ceilG)||1)*dt; /* ceilG : gravité de la balle une fois qu'elle a touché le plafond (flip reset : retombée lente) */
  b.vel.multiplyScalar(1-.02*dt);
  const sp=b.vel.length();if(sp>BMAX)b.vel.multiplyScalar(BMAX/sp);
  b.pos.addScaledVector(b.vel,dt);
  for(let it=0;it<3;it++){
    sdf(b.pos.x,b.pos.y,b.pos.z,S,true);
    const pen=S.d+BR;if(pen<=0)break;
    b.pos.x-=S.nx*pen;b.pos.y-=S.ny*pen;b.pos.z-=S.nz*pen;
    const vn=b.vel.x*S.nx+b.vel.y*S.ny+b.vel.z*S.nz;
    if(vn>0){
      const e=vn>4?b.rest:0;
      b.vel.x-=S.nx*vn*(1+e);b.vel.y-=S.ny*vn*(1+e);b.vel.z-=S.nz*vn*(1+e);
      b.vel.multiplyScalar(1-.3*dt);
      if(vn>12)SFX.hit(vn*.4);
      /* Dropshot : une balle chargée qui touche le sol abîme les dalles adverses */
      if(MODE.drop&&S.ny<-.7&&vn>.05&&clockT-b.lastT>.12)dropImpact(b.pos.x,b.pos.z);
    }
  }
  if(!(b.pos.x===b.pos.x)||Math.abs(b.pos.x)>150||Math.abs(b.pos.y)>80||Math.abs(b.pos.z)>150){resetBall();return;}
  checkScore(b);
}
function checkScore(b){
  if(phase!=='play')return;
  if(CM.cur&&CM.cur.goals&&CM.cur.goals.length&&cgCheck(b))return;
  if(MODE.hoops){
    for(let s=-1;s<=1;s+=2){
      const cz=s*(HZ-HOOP.rz);
      if(b.py>HOOP.ry&&b.pos.y<=HOOP.ry){
        const dx=b.pos.x,dz=b.pos.z-cz;
        if(dx*dx+dz*dz<(HOOP.Rc-.2)*(HOOP.Rc-.2)){onGoal(s>0?0:1);return;}
      }
    }
  }else if(MODE.drop){
    if(b.pos.y<-BR*1.5){const t=dropTileAt(b.pos.x,b.pos.z);onGoal(t&&t.team>=0?1-t.team:(b.pos.z>0?0:1));}
  }else if(HASGOAL&&Math.abs(b.pos.z)>HZ+BR*.85&&b.pos.y<GH+BR&&Math.abs(b.pos.x)<GW+BR)onGoal(b.pos.z>0?0:1);
}


/* ---------- enregistrement pour les replays ---------- */
const recBuf=[];let recCtr=0;
function record(){
  const a=new Float32Array(3+cars.length*8);
  a[0]=ball.pos.x;a[1]=ball.pos.y;a[2]=ball.pos.z;let k=3;
  for(const c of cars){a[k++]=c.pos.x;a[k++]=c.pos.y;a[k++]=c.pos.z;a[k++]=c.q.x;a[k++]=c.q.y;a[k++]=c.q.z;a[k++]=c.q.w;a[k++]=c.dead?-1:(c.boosting?1:0);}
  recBuf.push(a);if(recBuf.length>420)recBuf.shift();
}
let clockT=0;
function simStep(dt){
  clockT+=dt;
  if(MODE.moves&&MV.state==='demo')coachStep(dt);
  else if(MODE.moves&&MV.coach){const I=MV.coach.in;I.x=I.y=I.thr=I.roll=0;I.boost=I.jump=I.jumpEdge=I.drift=I.rollMode=false;}
  for(const c of cars)stepCar(c,dt);
  for(let i=0;i<cars.length;i++)for(let j=i+1;j<cars.length;j++)carCar(cars[i],cars[j]);
  if(ball.alive){stepBall(dt);for(const c of cars)carBall(c);}
  if(MODE.moves&&MV.state==='demo'&&!MV.S.won){const mm=MOVES[MV.i];if(mm&&mm.script)mm.script(MV.coach,dt,MV.S,MV.t);}
  if(MODE.moves)mvTick(dt);
  recCtr++;
  if(recCtr%2===0&&(phase==='play'||(phase==='goal'&&phaseT<1.1)))record();
}

/* =====================================================================
   INTELLIGENCE ARTIFICIELLE
   ===================================================================== */
function assignRoles(){
  for(let tm=0;tm<2;tm++){
    const atk=tm===0?1:-1,list=cars.filter(c=>c.team===tm&&!c.dead);
    list.forEach(c=>{
      tA.copy(ball.pos).sub(c.pos);tA.y=0;const d=tA.length();
      const toward=Math.max(0,c.vel.dot(tA.normalize()));
      c.eta=d/(14+toward)+((c.pos.z-ball.pos.z)*atk>6?2.5:0);
    });
    list.sort((a,b)=>a.eta-b.eta);list.forEach((c,i)=>{c.role=i;});
  }
}
/* buts pour les bots : cartes perso avec buts libres (attaque = buts qui rapportent un point à l'équipe, défense = buts de l'équipe) */
const BG={on:false,atk:[[],[]],def:[[],[]]};
function buildBotGoals(){
  BG.on=false;BG.atk=[[],[]];BG.def=[[],[]];
  const C=CM.cur;if(!C||!C.goals||!C.goals.length||MODE.free)return;
  const add=(mx,mz,fx,fz,hw,owner)=>{const e={mx:mx,mz:mz,fx:fx,fz:fz,hw:hw};
    for(let t=0;t<2;t++){if(owner===2||owner!==t)BG.atk[t].push(e);if(owner===t)BG.def[t].push(e);}};
  if(HASGOAL){add(0,-HZ,0,1,GW,0);add(0,HZ,0,-1,GW,1);}
  C.goals.forEach(g=>add(g.mx,g.mz,g.fx,g.fz,g.w/2,g.team));
  BG.on=true;
}
function botPick(list,bp){
  let best=null,bd=1e9;
  for(const g of list){let d=Math.hypot(bp.x-g.mx,bp.z-g.mz);if((bp.x-g.mx)*g.fx+(bp.z-g.mz)*g.fz<0)d+=60;if(d<bd){bd=d;best=g;}}
  return best;
}
function botThink(c,dt){
  const I=c.in;I.jump=false;I.boost=false;I.drift=false;I.rollMode=false;I.roll=0;I.x=0;I.y=0;I.thr=0;
  if(c.dead)return;
  if(MODE.rumble)botPower(c,dt);
  const sk=c.skill,atk=c.team===0?1:-1,ownZ=-atk*HZ;
  c.hitCool-=dt;c.aerialCool-=dt;c.think-=dt;
  if(c.think<=0){c.think=.3+rnd()*.3;c.boostOK=rnd()<(.3+sk*.7);   /* Rookie : boost rare ; All-Star : boost à fond */}
  if(c.revT>0){c.revT-=dt;I.thr=-1;I.x=c.revS;return;}
  const bp=ball.pos,bv=ball.vel;
  tA.copy(bp).sub(c.pos);const dist=tA.length()+1e-3;
  const fs=c.vel.dot(c.fwd);
  if(!c.grounded){
    if(c.aerial){
      c.aerialT-=dt;
      if(c.aerialT<=0||!ball.alive)c.aerial=false;
      else{
        I.jump=c.jumpT>0;
        tC.copy(bp).addScaledVector(bv,.25).sub(c.pos);const dl=tC.length()+1e-3;tC.divideScalar(dl);
        const lx=tC.dot(c.right),ly=tC.dot(c.up),lf=tC.dot(c.fwd);
        I.y=lf<0?(ly>=0?1:-1):clamp(ly*3,-1,1);I.x=clamp(lx*3,-1,1);I.roll=clamp(c.right.y*2.5,-1,1);
        if(lf>.92&&c.boost>1)I.boost=true;
        if(dl<7&&c.flipAvail&&c.flipClock>0&&c.jumpT<=0){I.x=0;I.y=-1;I.jumpEdge=true;I.jump=true;c.aerial=false;}
        return;
      }
    }
    if(c.botFlip>0){c.botFlip-=dt;if(c.botFlip<=0&&c.flipAvail&&c.flipClock>0){I.y=-1;I.x=0;I.jumpEdge=true;I.jump=true;}}
    return;
  }
  c.aerial=false;
  if(Math.abs(fs)<2&&phase==='play')c.stuck+=dt;else c.stuck=Math.max(0,c.stuck-dt*2);
  if(c.stuck>1.1){c.stuck=0;c.revT=.7;c.revS=rnd()<.5?-1:1;return;}
  let tgt=tB,boostMode=true;
  const role=c.role;
  if(role===0){
    const t=clamp(dist/40,0,1)*.8;
    tB.copy(bp).addScaledVector(bv,t);tB.y=0;
    tB.x=clamp(tB.x,-HX+6,HX-6);tB.z=clamp(tB.z,-HZ+4,HZ-4);if(DROP.on)hexFit(tB,4);
    const ag=BG.on?botPick(BG.atk[c.team],bp):null;
    if(ag)tC.set(ag.mx-ag.fx*3,0,ag.mz-ag.fz*3).sub(tB);
    else tC.set(0,0,MODE.hoops?atk*(HZ-HOOP.rz):atk*HZ).sub(tB);
    tC.y=0;tC.normalize();
    tD.copy(c.pos).sub(tB);tD.y=0;const behind=tD.dot(tC);
    if(behind<-3)tB.addScaledVector(tC,-(BR+2.2));
    else{
      const px=-tC.z,pz=tC.x,s=(tD.x*px+tD.z*pz)>=0?1:-1;
      tB.addScaledVector(tC,-(BR+14));tB.x+=px*s*9;tB.z+=pz*s*9;
    }
  }else if(BG.on){
    const dg=botPick(BG.def[c.team],bp);
    if(dg){
      const px=dg.mx+dg.fx*(role===1?10:18),pz=dg.mz+dg.fz*(role===1?10:18);
      if(role===1)tB.set(lerp(bp.x,px,.5),0,lerp(bp.z,pz,.5));
      else{const lat=clamp(((bp.x-px)*-dg.fz+(bp.z-pz)*dg.fx)*.4,-Math.min(10,dg.hw*.8),Math.min(10,dg.hw*.8));tB.set(px-dg.fz*lat,0,pz+dg.fx*lat);}
    }else tB.set(lerp(bp.x,0,role===1?.45:.7),0,lerp(bp.z,0,role===1?.45:.7));
  }else if(role===1){
    tB.set(bp.x*.5-Math.sign(bp.x||1)*8,0,lerp(bp.z,ownZ,.45));
  }else{
    tB.set(clamp(bp.x*.4,-14,14),0,ownZ*.78+bp.z*.12);
  }
  tB.x=clamp(tB.x,-HX+5,HX-5);tB.z=clamp(tB.z,-HZ+5,HZ-5);if(DROP.on)hexFit(tB,5);
  if(c.boost<22&&(role>0||dist>45)){const p=nearestPad(c.pos,role>0?90:50);if(p){tB.set(p.x,0,p.z);boostMode=false;}}
  tE.copy(tgt).sub(c.pos);
  const lx=tE.dot(c.right),lf=tE.dot(c.fwd),dd=Math.hypot(lx,lf),ang=Math.atan2(lx,lf);
  I.x=clamp(ang*(1.8+sk),-1,1);   /* Rookie : conduite moins précise ; All-Star : précision maximale */
  I.thr=(role===0||dd>16)?1:clamp((dd-3)/8,-.6,1);
  if(I.thr>0)I.thr*=.8+.2*Math.min(1,sk);   /* Rookie : roule plus mollement */
  I.drift=Math.abs(ang)>1.1&&dd>9&&fs>14;
  if(boostMode&&c.boostOK&&c.boost>6&&Math.abs(ang)<.2&&dd>16&&fs<46)I.boost=true;
  // frappe / flip sur la balle
  if(dist<10&&c.hitCool<=0){
    const face=tA.dot(c.fwd)/dist;
    if(face>.78&&sk>.35&&rnd()<(.03+.1*sk)){I.jump=true;I.jumpEdge=true;c.botFlip=.1+rnd()*.12;c.hitCool=1.4;}
  }
  // aérien
  if(sk>.6&&c.aerialCool<=0&&bp.y>6&&c.boost>30){
    tF.copy(tA);tF.y=0;const hl=tF.length();
    if(hl<24&&tF.dot(c.fwd)/(hl+1e-3)>.6&&rnd()<.03){c.aerial=true;c.aerialT=2;c.aerialCool=4;I.jump=true;I.jumpEdge=true;}
  }
}

/* =====================================================================
   AUDIO
   ===================================================================== */
let AC=null,master=null,engO=null,engO2=null,engF=null,engG=null,bstF=null,bstG=null,crowdG=null,noiseBuf=null;
function initAudio(){
  if(AC){if(AC.state==='suspended')AC.resume();return;}
  try{
    AC=new (window.AudioContext||window.webkitAudioContext)();
    master=AC.createGain();master.gain.value=SV.set.vol;master.connect(AC.destination);
    engO=AC.createOscillator();engO.type='sawtooth';engF=AC.createBiquadFilter();engF.type='lowpass';engF.frequency.value=420;
    engG=AC.createGain();engG.gain.value=0;engO.connect(engF);engF.connect(engG);engG.connect(master);engO.start();engO2=AC.createOscillator();engO2.connect(engF);engO2.start();
    noiseBuf=AC.createBuffer(1,AC.sampleRate*2,AC.sampleRate);const d=noiseBuf.getChannelData(0);for(let i=0;i<d.length;i++)d[i]=Math.random()*2-1;
    const mk=(f,type,g)=>{const s=AC.createBufferSource();s.buffer=noiseBuf;s.loop=true;const bf=AC.createBiquadFilter();bf.type=type;bf.frequency.value=f;const gg=AC.createGain();gg.gain.value=g;s.connect(bf);bf.connect(gg);gg.connect(master);s.start();return [bf,gg];};
    const a=mk(900,'bandpass',0);bstF=a[0];bstG=a[1];
    const b=mk(500,'lowpass',.05);crowdG=b[1];
  }catch(e){AC=null;}
}
function tone(f0,f1,dur,type,vol,delay){
  if(!AC)return;const t=AC.currentTime+(delay||0),o=AC.createOscillator(),g=AC.createGain();
  o.type=type||'sine';o.frequency.setValueAtTime(f0,t);o.frequency.exponentialRampToValueAtTime(Math.max(20,f1),t+dur);
  g.gain.setValueAtTime(vol,t);g.gain.exponentialRampToValueAtTime(.0001,t+dur);o.connect(g);g.connect(master);o.start(t);o.stop(t+dur+.03);
}
function noiseHit(dur,vol,f){
  if(!AC||!noiseBuf)return;const s=AC.createBufferSource();s.buffer=noiseBuf;const bf=AC.createBiquadFilter();bf.type='lowpass';bf.frequency.value=f;
  const g=AC.createGain(),t=AC.currentTime;g.gain.setValueAtTime(vol,t);g.gain.exponentialRampToValueAtTime(.0001,t+dur);s.connect(bf);bf.connect(g);g.connect(master);s.start(t);s.stop(t+dur+.05);
}
let lastHitSfx=0;
const SFX={
  hit:v=>{const n=performance.now();if(n-lastHitSfx<70)return;lastHitSfx=n;tone(180,60,.18,'sine',Math.min(.6,.1+v*.02));noiseHit(.12,Math.min(.5,v*.015),1800);},
  jump:()=>tone(300,520,.12,'square',.05),
  pad:big=>tone(big?500:800,big?900:1400,.18,'sine',.12),
  beep:hi=>tone(hi?880:440,hi?880:440,hi?.5:.18,'square',.1),
  goal:()=>{[220,277,330,440].forEach((f,i)=>tone(f,f*1.02,1.4,'sawtooth',.07,i*.04));noiseHit(1.2,.3,2500);
    if(crowdG&&AC){crowdG.gain.cancelScheduledValues(AC.currentTime);crowdG.gain.setValueAtTime(.35,AC.currentTime);crowdG.gain.exponentialRampToValueAtTime(.05,AC.currentTime+4);}},
  demo:()=>{noiseHit(.5,.5,900);tone(120,40,.5,'sawtooth',.15);},
  /* le « ding » du flip reset : deux notes montantes + un petit scintillement */
  reset:()=>{tone(660,990,.22,'sine',.14);tone(990,1480,.32,'triangle',.1,.09);tone(1760,2200,.18,'sine',.05,.16);}
};
function updateAudio(){
  if(!AC||!player||phase==='menu'||phase==='paused'){if(engG)engG.gain.value=0;if(bstG)bstG.gain.value=0;return;}
  const t=AC.currentTime,sp=player.vel.length();
  const ad=IT['eng:'+player.L.eng].ad;if(engO.type!==ad.t1)engO.type=ad.t1;if(engO2.type!==ad.t2)engO2.type=ad.t2;const ef=ad.base+sp*ad.k+(player.boosting?30:0);engO.frequency.setTargetAtTime(ef,t,.06);engO2.frequency.setTargetAtTime(ef*ad.r2,t,.06);engF.frequency.setTargetAtTime(ad.cut*.7+sp*14,t,.06);
  engG.gain.setTargetAtTime(player.dead?0:(.05+.06*Math.abs(player.in.thr)),t,.08);
  bstG.gain.setTargetAtTime(player.boosting?.22:0,t,.05);
  if(bstF)bstF.frequency.setTargetAtTime(player.boosting?700+sp*12:900,t,.08);
  if(crowdG&&phase!=='goal')crowdG.gain.setTargetAtTime(.05+(ball.alive?Math.min(.05,ball.vel.length()*.0012):0),t,.4);
}

/* =====================================================================
   MUSIQUE DU MENU : 4 morceaux originaux générés en direct (WebAudio), sans aucun fichier audio.
   Chaque morceau = 8 mesures qui tournent 3 fois (la 2e moitié ajoute la mélodie), puis on passe au suivant.
   Batterie, basse, nappes (qui « pompent » sur la grosse caisse), arpège et mélodie avec un écho.
   La musique ne joue que dans le menu ; elle se coupe en match. Toucher le bandeau « MUSIQUE » passe au morceau suivant.
   ===================================================================== */
const MUS={on:false,out:null,comp:null,bus:null,pad:null,fx:null,k:0,order:[0,1,2,3],tr:0,step:0,next:0,loop:0,timer:null,name:'',spb:.1};
/* K/S/H : grosse caisse, caisse claire, charleston (16 pas par mesure). B : basse (1 fondamentale, 2 octave, 3 quinte). A : arpège (0-2 notes de l'accord, 3 fondamentale+2 octaves).
   L : mélodie, une note par croche sur 2 mesures (chiffre = degré de la gamme pentatonique mineure, - = silence). prog : [décalage de la fondamentale, 'm' mineur / 'M' majeur] */
const MTR=[
 {n:'Neon Garage',bpm:118,root:57,prog:[[0,'m'],[-4,'M'],[3,'M'],[-2,'M']],
  K:'1000100010001000',S:'0000100000001000',H:'0010001000100010',B:'1010201010103020',A:'0-12-32-1-02-31-',L:'5-43-2-0-34-3-2-',wave:'sawtooth'},
 {n:'Turbo Drift',bpm:128,root:50,prog:[[0,'m'],[-4,'M'],[3,'M'],[-2,'M']],
  K:'1000001000100000',S:'0000100000001001',H:'1011101110111011',B:'1001001010020010',A:'0120120301201203',L:'7-65-3-5-67-5-3-',wave:'square'},
 {n:'Stadium Lights',bpm:100,root:52,prog:[[0,'m'],[-4,'M'],[3,'M'],[-2,'M']],
  K:'1000000010100000',S:'0000000010000000',H:'1010101010101010',B:'1000000200100000',A:'0-1-2-1-0-1-2-3-',L:'4---5-4-2---0-2-',wave:'triangle'},
 {n:'Overtime',bpm:140,root:54,prog:[[0,'m'],[-4,'M'],[3,'M'],[-2,'M']],
  K:'1000100010001000',S:'0000100000001010',H:'1111111111111111',B:'1121112111211121',A:'0123210301232103',L:'6-75-4-5-67-9-7-',wave:'sawtooth'}
];
const PENT=[0,3,5,7,10];
const mtof=m=>440*Math.pow(2,(m-69)/12);
function muEnv(g,t,a,peak,d){g.gain.setValueAtTime(.0001,t);g.gain.linearRampToValueAtTime(peak,t+a);g.gain.exponentialRampToValueAtTime(.0001,t+a+d);}
function muKick(t){
  const o=AC.createOscillator(),g=AC.createGain();o.type='sine';o.frequency.setValueAtTime(160,t);o.frequency.exponentialRampToValueAtTime(42,t+.13);
  g.gain.setValueAtTime(.95,t);g.gain.exponentialRampToValueAtTime(.001,t+.36);o.connect(g);g.connect(MUS.bus);o.start(t);o.stop(t+.4);
  const p=MUS.pad.gain;p.cancelScheduledValues(t);p.setValueAtTime(.4,t);p.linearRampToValueAtTime(1,t+.22);   /* effet « pompe » sur les nappes */
}
function muNoise(t,type,f,vol,dec,q){
  const sr=AC.createBufferSource();sr.buffer=noiseBuf;const bf=AC.createBiquadFilter();bf.type=type;bf.frequency.value=f;if(q)bf.Q.value=q;
  const g=AC.createGain();g.gain.setValueAtTime(vol,t);g.gain.exponentialRampToValueAtTime(.001,t+dec);sr.connect(bf);bf.connect(g);g.connect(MUS.bus);sr.start(t,Math.random()*1.5);sr.stop(t+dec+.05);
}
function muSnare(t,v){muNoise(t,'bandpass',1900,.42*v,.2,.8);const o=AC.createOscillator(),g=AC.createGain();o.type='triangle';o.frequency.setValueAtTime(210,t);o.frequency.exponentialRampToValueAtTime(120,t+.1);
  g.gain.setValueAtTime(.3*v,t);g.gain.exponentialRampToValueAtTime(.001,t+.12);o.connect(g);g.connect(MUS.bus);o.start(t);o.stop(t+.15);}
function muHat(t,v){muNoise(t,'highpass',7500,.13*v,.045);}
function muBass(t,f,d,wave){
  const o=AC.createOscillator(),o2=AC.createOscillator(),fl=AC.createBiquadFilter(),g=AC.createGain();
  o.type=wave==='square'?'square':'sawtooth';o2.type='sine';o.frequency.value=f;o2.frequency.value=f;
  fl.type='lowpass';fl.Q.value=4;fl.frequency.setValueAtTime(1100,t);fl.frequency.exponentialRampToValueAtTime(220,t+d);
  g.gain.setValueAtTime(.0001,t);g.gain.linearRampToValueAtTime(.3,t+.012);g.gain.setValueAtTime(.3,t+d*.7);g.gain.exponentialRampToValueAtTime(.0001,t+d);
  o.connect(fl);fl.connect(g);o2.connect(g);g.connect(MUS.bus);o.start(t);o2.start(t);o.stop(t+d+.05);o2.stop(t+d+.05);
}
function muPad(t,notes,d){
  const fl=AC.createBiquadFilter(),g=AC.createGain();fl.type='lowpass';fl.frequency.value=1500;
  g.gain.setValueAtTime(.0001,t);g.gain.linearRampToValueAtTime(.075,t+.7);g.gain.setValueAtTime(.075,t+d-.5);g.gain.linearRampToValueAtTime(.0001,t+d+.2);
  fl.connect(g);g.connect(MUS.pad);
  for(const m of notes)for(const dt of [-8,8]){const o=AC.createOscillator();o.type='sawtooth';o.frequency.value=mtof(m);o.detune.value=dt;o.connect(fl);o.start(t);o.stop(t+d+.3);}
}
function muPluck(t,f,d,wave){
  const o=AC.createOscillator(),fl=AC.createBiquadFilter(),g=AC.createGain();o.type=wave==='triangle'?'triangle':'square';o.frequency.value=f;
  fl.type='lowpass';fl.frequency.setValueAtTime(3600,t);fl.frequency.exponentialRampToValueAtTime(500,t+d);
  g.gain.setValueAtTime(.0001,t);g.gain.linearRampToValueAtTime(.07,t+.006);g.gain.exponentialRampToValueAtTime(.0001,t+d);
  o.connect(fl);fl.connect(g);g.connect(MUS.pad);g.connect(MUS.fx);o.start(t);o.stop(t+d+.05);
}
function muLead(t,f,d,wave){
  const o=AC.createOscillator(),o2=AC.createOscillator(),fl=AC.createBiquadFilter(),g=AC.createGain();
  o.type=wave;o2.type='triangle';o.frequency.value=f;o2.frequency.value=f*2;o.detune.value=5;
  fl.type='lowpass';fl.frequency.value=2800;
  g.gain.setValueAtTime(.0001,t);g.gain.linearRampToValueAtTime(.085,t+.02);g.gain.setValueAtTime(.085,t+d*.6);g.gain.exponentialRampToValueAtTime(.0001,t+d);
  o.connect(fl);o2.connect(fl);fl.connect(g);g.connect(MUS.pad);g.connect(MUS.fx);o.start(t);o2.start(t);o.stop(t+d+.05);o2.stop(t+d+.05);
}
function musStep(T,st,t){
  const spb=MUS.spb,bar=(st>>4)&7,i=st&15,sec=bar>=4,ch=T.prog[bar&3];
  const rm=T.root+ch[0],iv=ch[1]==='m'?[0,3,7]:[0,4,7];
  if(T.K[i]==='1')muKick(t);
  if(T.S[i]==='1')muSnare(t,1);
  if(bar===7&&i>=12)muSnare(t,.35+(i-12)*.15);   /* petit roulement avant de reboucler */
  if(T.H[i]==='1')muHat(t,i%4===2?1:.55);
  if(sec&&i%4===3&&T.H[i]!=='1')muHat(t,.4);
  const b=T.B[i];if(b!=='0')muBass(t,mtof(rm-12+(b==='2'?12:b==='3'?7:0)),spb*(T.bpm>=128?1.6:2),T.wave);
  if(i===0)muPad(t,[rm,rm+iv[1],rm+iv[2],rm+12],spb*16);
  const a=T.A[i];if(a!=='-'){const k=+a;muPluck(t,mtof(k<3?rm+12+iv[k]:rm+24),spb*1.6,T.wave);}
  if(sec&&i%2===0){const d=T.L[(((bar&1)<<4)+i)>>1];if(d!==undefined&&d!=='-'){const dg=+d;muLead(t,mtof(T.root+12+PENT[dg%5]+12*Math.floor(dg/5)),spb*3.2,T.wave==='square'?'sawtooth':T.wave);}}
}
function musBuild(){
  const T=MTR[MUS.tr],t=AC.currentTime;
  if(MUS.bus){const ob=MUS.bus;ob.gain.cancelScheduledValues(t);ob.gain.setTargetAtTime(0,t,.07);setTimeout(()=>{try{ob.disconnect();}catch(e){}},1600);}
  const bus=AC.createGain(),pad=AC.createGain(),fx=AC.createGain(),dl=AC.createDelay(2),fb=AC.createGain(),df=AC.createBiquadFilter();
  MUS.spb=60/T.bpm/4;
  dl.delayTime.value=MUS.spb*3;fb.gain.value=.34;df.type='lowpass';df.frequency.value=2400;
  fx.gain.value=.38;fx.connect(dl);dl.connect(df);df.connect(fb);fb.connect(dl);df.connect(bus);
  pad.connect(bus);bus.connect(MUS.comp);
  MUS.bus=bus;MUS.pad=pad;MUS.fx=fx;MUS.name=T.n;
  musUI();
}
function musUI(){const m=$('mtitle');if(m)m.textContent='♪ '+MUS.name;}
function musNext(){
  MUS.k=(MUS.k+1)%MUS.order.length;MUS.tr=MUS.order[MUS.k];MUS.step=0;MUS.loop=0;
  musBuild();MUS.next=AC.currentTime+.06;
}
function musSched(){
  if(!AC||!MUS.on)return;
  while(MUS.next<AC.currentTime+.2){
    musStep(MTR[MUS.tr],MUS.step,MUS.next);MUS.next+=MUS.spb;MUS.step++;
    if(MUS.step>=128){MUS.step=0;MUS.loop++;if(MUS.loop>=3){musNext();break;}}
  }
}
function musVol(){return (SV.set.mvol==null?.7:SV.set.mvol)*.9;}
function musApplyVol(){if(MUS.out&&MUS.on)MUS.out.gain.setTargetAtTime(musVol(),AC.currentTime,.05);}
function musStart(){
  if(!MUS.out){
    MUS.out=AC.createGain();MUS.out.gain.value=0;MUS.comp=AC.createDynamicsCompressor();MUS.comp.threshold.value=-14;MUS.comp.ratio.value=4;MUS.comp.attack.value=.01;MUS.comp.release.value=.2;
    MUS.comp.connect(MUS.out);MUS.out.connect(master);
    MUS.order=[0,1,2,3].sort(()=>Math.random()-.5);MUS.k=-1;
  }
  MUS.on=true;MUS.err=null;
  musNext();
  MUS.out.gain.cancelScheduledValues(AC.currentTime);MUS.out.gain.setTargetAtTime(musVol(),AC.currentTime,.5);
  clearInterval(MUS.timer);MUS.timer=setInterval(musSched,40);
}
function musStop(){
  MUS.on=false;clearInterval(MUS.timer);MUS.timer=null;
  if(MUS.out)MUS.out.gain.setTargetAtTime(0,AC.currentTime,.12);
  if(MUS.bus){const ob=MUS.bus;setTimeout(()=>{if(!MUS.on)try{ob.disconnect();}catch(e){}},1200);}
}
function musHint(){
  const l=$('mlab');if(!l)return;
  const t=!AC||AC.state!=='running'?(MUS.err?'MUSIQUE : ERREUR (voir console)':'TOUCHE L\'ÉCRAN POUR LE SON'):'MUSIQUE DU MENU';
  if(l.textContent!==t)l.textContent=t;
}
function musTick(){
  try{
    /* on lance la musique même si le navigateur a encore le son bloqué : elle démarre toute seule au premier toucher */
    const want=phase==='menu'&&!document.hidden&&!!AC&&!INTRO.on;
    if(want&&!MUS.on&&!MUS.err)musStart();else if(!want&&MUS.on)musStop();
    if(AC&&AC.state==='suspended'&&MUS.on)AC.resume().catch(()=>{});
    musHint();
  }catch(e){MUS.err=e.message||String(e);MUS.on=false;clearInterval(MUS.timer);console.error('Musique du menu :',e);musHint();}
}
/* le navigateur n'autorise le son qu'après un toucher / une touche : on débloque l'audio dès le premier geste */
['pointerdown','pointerup','click','keydown','touchstart','touchend'].forEach(ev=>document.addEventListener(ev,()=>{try{initAudio();if(AC&&AC.state!=='running')AC.resume().catch(()=>{});}catch(e){}},{passive:true,capture:true}));

/* =====================================================================
   ÉCRAN DE VEILLE : après un moment sans rien toucher dans le menu, un écran sombre prend le relais
   (étoiles qui défilent, titre qui rebondit sur les bords en changeant de couleur, balle qui rebondit, heure).
   Le rendu 3D du menu est suspendu pendant ce temps (économie de batterie). Toucher, bouger la souris, une touche
   ou un bouton de manette ramène au menu. Le délai se règle dans les réglages (ou « Désactivé »).
   ===================================================================== */
const SAVER={on:false,last:performance.now(),gpT:0,el:null,cv:null,cx:null,raf:0,st:null,mx:0,my:0,lastF:0};
function saverDelay(){const v=SV.set.saver;return v==null?60:+v;}
['pointerdown','pointermove','keydown','touchstart','wheel','click'].forEach(ev=>document.addEventListener(ev,()=>{if(!SAVER.on)SAVER.last=performance.now();},{passive:true,capture:true}));
function saverPad(){
  try{const gp=navigator.getGamepads?navigator.getGamepads():[];
    for(const g of gp){if(g&&(g.buttons.some(b=>b.pressed)||g.axes.some(a=>Math.abs(a)>.6)))return true;}}catch(e){}
  return false;
}
function saverTick(){
  const now=performance.now();
  if(now-SAVER.gpT>250){SAVER.gpT=now;if(saverPad()){if(SAVER.on)saverStop();else SAVER.last=now;}}
  if(SAVER.on){if(phase!=='menu')saverStop();return;}
  const d=saverDelay();
  if(!d||phase!=='menu'||INTRO.on||GATE.on||document.hidden||$('lfx')){SAVER.last=now;return;}
  if(now-SAVER.last>d*1000)saverStart();
}
function saverInit(){
  const cv=SAVER.cv,dpr=Math.min(2,window.devicePixelRatio||1),W=innerWidth,H=innerHeight;
  cv.width=Math.round(W*dpr);cv.height=Math.round(H*dpr);cv.style.width=W+'px';cv.style.height=H+'px';
  const u=Math.min(W,H),old=SAVER.st;
  const st={W:W,H:H,dpr:dpr,u:u,t:0,ci:0,font:getComputedStyle(document.body).fontFamily||'sans-serif',
    stars:old?old.stars:Array.from({length:110},()=>({x:Math.random(),y:Math.random(),z:.2+Math.random()*.8})),
    lx:old?old.lx:W*.3,ly:old?old.ly:H*.3,lvx:old?old.lvx:u*.16,lvy:old?old.lvy:u*.11,lw:0,lh:0,
    bx:old?old.bx:W*.7,by:old?old.by:H*.4,bvx:old?old.bvx:u*.35,bvy:old?old.bvy:0,br:u*.032,trail:[]};
  SAVER.st=st;
}
function saverStart(){
  if(SAVER.on)return;
  SAVER.on=true;SAVER.st=null;
  const el=document.createElement('div');el.id='saver';
  el.style.cssText='position:fixed;inset:0;z-index:95;background:#03050e;opacity:0;transition:opacity .8s;cursor:none;touch-action:none';
  const cv=document.createElement('canvas');el.appendChild(cv);document.body.appendChild(el);
  SAVER.el=el;SAVER.cv=cv;SAVER.cx=cv.getContext('2d');SAVER.mx=SAVER.my=-1;
  saverInit();void el.offsetWidth;el.style.opacity='1';
  const wake=e=>{e.stopPropagation();e.preventDefault();saverStop();};
  el.addEventListener('pointerdown',wake);el.addEventListener('touchstart',wake,{passive:false});el.addEventListener('wheel',wake,{passive:false});el.addEventListener('click',wake);
  el.addEventListener('pointermove',e=>{if(SAVER.mx<0){SAVER.mx=e.clientX;SAVER.my=e.clientY;return;}if(Math.hypot(e.clientX-SAVER.mx,e.clientY-SAVER.my)>8)saverStop();});
  SAVER.lastF=performance.now();SAVER.raf=requestAnimationFrame(saverFrame);
}
function saverStop(){
  if(!SAVER.on)return;
  SAVER.on=false;SAVER.last=performance.now();cancelAnimationFrame(SAVER.raf);
  const el=SAVER.el;SAVER.el=null;
  if(el){el.style.transition='opacity .25s';el.style.opacity='0';setTimeout(()=>el.remove(),280);}
}
document.addEventListener('keydown',e=>{if(SAVER.on){e.stopPropagation();e.preventDefault();saverStop();}},true);
window.addEventListener('resize',()=>{if(SAVER.on&&SAVER.cv)saverInit();});
const SAVER_COLS=[['#2f8bff','#7df9ff'],['#ff8a2a','#ffd166'],['#7dffb0','#2fd1a0'],['#ff4fd8','#ffd0f4'],['#a24bff','#d9b8ff']];
function saverFrame(now){
  if(!SAVER.on)return;
  SAVER.raf=requestAnimationFrame(saverFrame);
  const st=SAVER.st,cx=SAVER.cx;let dt=Math.min(.05,(now-SAVER.lastF)/1000);SAVER.lastF=now;if(!(dt>0))dt=.016;
  st.t+=dt;const W=st.W,H=st.H,u=st.u;
  cx.setTransform(st.dpr,0,0,st.dpr,0,0);
  /* fond + lueur lente */
  const g=cx.createLinearGradient(0,0,0,H);g.addColorStop(0,'#040714');g.addColorStop(1,'#0a1230');cx.fillStyle=g;cx.fillRect(0,0,W,H);
  const gx=W*(.5+.3*Math.sin(st.t*.13)),gy=H*(.55+.2*Math.cos(st.t*.1));
  const rg=cx.createRadialGradient(gx,gy,0,gx,gy,u*.9);rg.addColorStop(0,'rgba(47,139,255,.16)');rg.addColorStop(1,'rgba(47,139,255,0)');cx.fillStyle=rg;cx.fillRect(0,0,W,H);
  const gx2=W*(.5+.3*Math.cos(st.t*.11)),gy2=H*(.45+.2*Math.sin(st.t*.09));
  const rg2=cx.createRadialGradient(gx2,gy2,0,gx2,gy2,u*.8);rg2.addColorStop(0,'rgba(255,138,42,.11)');rg2.addColorStop(1,'rgba(255,138,42,0)');cx.fillStyle=rg2;cx.fillRect(0,0,W,H);
  /* étoiles qui défilent */
  for(const s of st.stars){
    s.x-=dt*.012*s.z;if(s.x<0){s.x=1;s.y=Math.random();}
    cx.globalAlpha=.25+.55*s.z*(.6+.4*Math.sin(st.t*2+s.y*40));cx.fillStyle='#cfe0ff';cx.fillRect(s.x*W,s.y*H,1+s.z*1.6,1+s.z*1.6);
  }
  cx.globalAlpha=1;
  /* balle : rebonds avec gravité, traînée */
  st.bvy+=u*1.1*dt;st.bx+=st.bvx*dt;st.by+=st.bvy*dt;
  const br=st.br;
  if(st.bx<br){st.bx=br;st.bvx=Math.abs(st.bvx);}else if(st.bx>W-br){st.bx=W-br;st.bvx=-Math.abs(st.bvx);}
  if(st.by>H-br){st.by=H-br;st.bvy=-Math.max(Math.abs(st.bvy)*.88,u*.55+Math.random()*u*.25);}
  if(st.by<br){st.by=br;st.bvy=Math.abs(st.bvy)*.5;}
  st.trail.push([st.bx,st.by]);if(st.trail.length>22)st.trail.shift();
  for(let i=0;i<st.trail.length;i++){const a=i/st.trail.length;cx.globalAlpha=a*.28;cx.fillStyle='#9ec4ff';cx.beginPath();cx.arc(st.trail[i][0],st.trail[i][1],br*(.35+.5*a),0,6.283);cx.fill();}
  cx.globalAlpha=1;
  const bg=cx.createRadialGradient(st.bx-br*.35,st.by-br*.4,br*.1,st.bx,st.by,br);bg.addColorStop(0,'#ffffff');bg.addColorStop(.45,'#dfe6f5');bg.addColorStop(1,'#7d8cae');
  cx.shadowColor='rgba(150,190,255,.8)';cx.shadowBlur=br*.9;cx.fillStyle=bg;cx.beginPath();cx.arc(st.bx,st.by,br,0,6.283);cx.fill();cx.shadowBlur=0;
  cx.strokeStyle='rgba(40,60,110,.55)';cx.lineWidth=br*.1;cx.beginPath();cx.arc(st.bx,st.by,br*.45,0,6.283);cx.stroke();
  /* titre qui rebondit sur les bords et change de couleur à chaque rebond */
  const fs=Math.round(u*.085);cx.font='italic 900 '+fs+'px '+st.font;
  const txt='ROCKET ARENA 3D',tw=cx.measureText(txt).width;st.lw=tw+fs*.6;st.lh=fs*1.9;
  st.lx+=st.lvx*dt;st.ly+=st.lvy*dt;let hit=false;
  if(st.lx<0){st.lx=0;st.lvx=Math.abs(st.lvx);hit=true;}else if(st.lx>W-st.lw){st.lx=Math.max(0,W-st.lw);st.lvx=-Math.abs(st.lvx);hit=true;}
  if(st.ly<0){st.ly=0;st.lvy=Math.abs(st.lvy);hit=true;}else if(st.ly>H-st.lh){st.ly=Math.max(0,H-st.lh);st.lvy=-Math.abs(st.lvy);hit=true;}
  if(hit)st.ci=(st.ci+1)%SAVER_COLS.length;
  const col=SAVER_COLS[st.ci],x=st.lx+fs*.3,y=st.ly;
  const tg=cx.createLinearGradient(x,0,x+tw,0);tg.addColorStop(0,col[0]);tg.addColorStop(1,col[1]);
  cx.textBaseline='alphabetic';cx.shadowColor=col[0];cx.shadowBlur=fs*.6;cx.fillStyle=tg;cx.fillText(txt,x,y+fs*1.15);cx.shadowBlur=0;
  const d=new Date(),hm=String(d.getHours()).padStart(2,'0')+':'+String(d.getMinutes()).padStart(2,'0');
  cx.font='700 '+Math.round(fs*.38)+'px '+st.font;cx.fillStyle='rgba(255,255,255,.7)';cx.fillText(hm,x,y+fs*1.7);
  const pulse=.35+.35*Math.sin(st.t*2.2);cx.globalAlpha=pulse;cx.font='700 '+Math.round(fs*.26)+'px '+st.font;cx.fillText('TOUCHE L\'ÉCRAN POUR REVENIR',x+fs*1.5,y+fs*1.7);cx.globalAlpha=1;
}

/* =====================================================================
   HUD / MESSAGES
   ===================================================================== */
function feed(text,css){
  const d=document.createElement('div');d.textContent=text;d.style.borderLeftColor=css||'#fff';el.feed.appendChild(d);
  while(el.feed.children.length>4)el.feed.removeChild(el.feed.firstChild);
  setTimeout(()=>{if(d.parentNode)d.parentNode.removeChild(d);},4500);
}
function popup(text,pts,css){
  const box=$('pops');if(!box)return;
  const d=document.createElement('div');d.style.color=css||'#fff';d.textContent=text;
  if(pts){const b=document.createElement('b');b.textContent='+'+pts;d.appendChild(b);}
  box.appendChild(d);while(box.children.length>4)box.removeChild(box.firstChild);
  setTimeout(()=>{if(d.parentNode)d.parentNode.removeChild(d);},2300);
}
function statRows(){
  return cars.slice().sort((a,b)=>b.stats.score-a.stats.score).map(c=>
    '<tr class="'+(c.team===0?'tb2':'to2')+'"><td>'+c.name+'</td><td>'+c.stats.score+'</td><td>'+c.stats.goals+'</td><td>'+c.stats.assists+'</td><td>'+c.stats.saves+'</td><td>'+c.stats.shots+'</td><td>'+c.stats.demos+'</td></tr>').join('');
}
function showScoreboard(on){
  const sb=$('sb');if(!sb)return;
  if(on&&(phase==='play'||phase==='countdown'||phase==='goal')){
    $('sbT').textContent='BLEU '+scores[0]+' – '+scores[1]+' ORANGE';
    $('sbTable').innerHTML='<table><tr><th>JOUEUR</th><th>SCORE</th><th>BUTS</th><th>PASSES</th><th>ARRÊTS</th><th>TIRS</th><th>DÉMOS</th></tr>'+statRows()+'</table>';
    sb.classList.add('show');
  }else sb.classList.remove('show');
}
function showMsg(big,sub,css,dur){
  el.mbig.textContent=big;el.mbig.style.color=css||'#fff';el.msub.textContent=sub||'';
  el.mbig.classList.remove('pop');void el.mbig.offsetWidth;el.mbig.classList.add('pop');msgT=dur||0;
}
function clearMsg(){el.mbig.textContent='';el.msub.textContent='';msgT=0;}
let _sb=-1,_so=-1,_clk='',_bn=-1,_sf=0;
function updateHUD(){
  if(scores[0]!==_sb){_sb=scores[0];el.sB.textContent=_sb;}
  if(scores[1]!==_so){_so=scores[1];el.sO.textContent=_so;}
  const t=overtime?otTime:Math.max(0,timeLeft),m=Math.floor(t/60),s=Math.floor(t%60),str=t>1e8?'∞':m+':'+(s<10?'0':'')+s;
  const sh=MODE.park?pkFmt(PK.t):(MODE.run?pkFmt(GR.t):str);if(sh!==_clk){_clk=sh;el.clock.textContent=sh;}
  el.ot.textContent=overtime?'PROLONGATION':'';
  if(player){
    const b=Math.round(player.boost);
    if(b!==_bn){_bn=b;el.ring.style.setProperty('--p',b);el.bnum.textContent=MUT.boost==='inf'?'∞':b;}
    if((_sf++)%6===0){
      const sp=player.vel.length();el.spd.textContent=Math.round(sp*2.7)+' km/h';
      const ss=sp>=45&&!player.dead;
      $('sprint').style.opacity=ss?1:0;$('spdl').style.opacity=sp>40?Math.min(1,(sp-40)/14):0;
    }
  }
}

/* =====================================================================
   DÉROULEMENT DU MATCH
   ===================================================================== */
function clearCars(){
  cars.forEach(c=>{scene.remove(c.mesh);scene.remove(c.shadow);if(c.beam)scene.remove(c.beam);});cars=[];player=null;
}
let lastGoal={team:0,ed:null,pos:V3()},freeT=0,unlimited=false;
function startMatch(){
  if(cfg.mode==='goalrun')CM.pend=grCompile();
  const isRun=cfg.mode==='goalrun'||!!(CM.pend&&CM.pend.seq); /* carte du créateur / de la communauté avec séquence de buts */
  CM.cur=CM.pend||null;CM.pend=null;
  initAudio();applyVolume();_bn=-1;_clk='';
  const k=isRun?'free':(cfg.mode||'casual');
  clearPowers();clearCars();
  GR.on=false;MODE.run=false;
  configureMode(k);
  MODE.run=isRun;
  MODE.park=!!(CM.cur&&CM.cur.mode==='parkour');PK.t=0;PK.run=false;PK.done=0;
  document.querySelectorAll('#score .tm').forEach(e=>{e.style.display=isRun?'none':'';});
  $('grHud').classList.toggle('hide',!isRun);
  if(isRun)grInit();
  hideUI();curtain(700);
  const sk=cfg.skill>0?cfg.skill:[.3,.65,1][cfg.diff||0],names=BOTN.slice().sort(()=>rnd()-.5);let ni=0;
  const solo=MODE.free||MODE.train||MODE.moves,myTeam=solo?0:cfg.team;cfg.team=myTeam;
  const sz=cfg.size;
  for(let tm=0;tm<2;tm++){
    const n=solo?(tm===myTeam?1:0):sz;
    for(let i=0;i<n;i++){
      const human=(tm===myTeam&&i===0);
      const pn=(!human&&tm===myTeam&&cfg.party&&cfg.party[i-1])||null;
      const c=newCar(tm,i,human,human?'TOI':(pn||names[ni++%names.length]),human?.8:(pn?((cfg.partySk&&cfg.partySk[i-1])||sk):sk));cars.push(c);if(human)player=c;
    }
  }
  scores=[0,0];unlimited=!cfg.dur||solo;timeLeft=unlimited?1e9:cfg.dur*60;overtime=false;otTime=0;recBuf.length=0;
  el.end.classList.remove('show');el.pause.classList.remove('show');
  el.hud.classList.remove('hide');if(isTouch)el.touch.classList.remove('hide');
  $('bPower').classList.toggle('hide',!MODE.rumble);
  buildBotGoals();
  resetPads();kickoff();
  const f=player.fwd;camHead.set(f.x,0,f.z).normalize();camPos.copy(player.pos).addScaledVector(camHead,-12);camPos.y=5;
  if(MODE.train){TR.att=0;TR.ok=0;trainSetup();phase='play';}
  $('mv').classList.add('hide');
  if(MODE.moves){phase='play';mvInit();}
  if(MODE.run){phase='play';grSpawn();showMsg(String((CM.cur&&CM.cur.name)||'PARCOURS DE BUTS').toUpperCase(),'Marque dans le but vert • R : replacer la balle',"#7dffb0",3.2);}
  else if(MODE.free){phase='play';if(MODE.park)showMsg('PARKOUR','Le chrono démarre quand tu bouges • R : recommencer',"#7dffb0",3);else showMsg('ENTRAÎNEMENT LIBRE','R : remettre la balle',"#6db0ff",2.4);}
}
function kickoff(){
  cars.forEach(placeCar);resetBall();resetPads();clearSkids();
  if(MODE.drop)dropRepair(true);
  clearPowers();
  phase='countdown';phaseT=0;countT=3.999;lastCount=4;
  clearMsg();recBuf.length=0;
}
/* ---------- parkour : chrono + arrivée ---------- */
function pkFmt(t){const m=Math.floor(t/60),s=t-m*60;return m+':'+(s<10?'0':'')+s.toFixed(2);}
function pkBest(){try{return JSON.parse(localStorage.getItem(PK.KEY))||{};}catch(e){return {};}}
function parkReset(){PK.t=0;PK.run=false;PK.done=0;}
function parkUpdate(dt){
  if(!player)return;
  if(player.pos.y<-30&&!PK.done){placeCar(player);parkReset();showMsg('TOMBÉ !','On recommence',"#ff8a8a",1.2);return;}
  if(PK.done){PK.done-=dt;if(PK.done<=0){placeCar(player);parkReset();clearMsg();}return;}
  if(!PK.run&&(player.vel.length()>1.5||player.in.thr!==0))PK.run=true;
  if(PK.run)PK.t+=dt;
  const C=CM.cur;if(!C||!C.fins)return;
  for(const f of C.fins){
    const dx=player.pos.x-f.x,dz=player.pos.z-f.z,dy=player.pos.y-f.y0;
    const lx=dx*f.cy-dz*f.sy,lz=dx*f.sy+dz*f.cy;
    if((f.N?polyIn(f,lx,lz,0):(Math.abs(lx)<f.w/2&&Math.abs(lz)<f.d/2))&&dy>-1&&dy<f.h){
      PK.done=4;PK.run=false;
      const b=pkBest(),k=C.name||'carte',old=b[k],rec=!(old<=PK.t);
      if(rec){b[k]=+PK.t.toFixed(2);try{localStorage.setItem(PK.KEY,JSON.stringify(b));}catch(e){}}
      showMsg('ARRIVÉE !',pkFmt(PK.t)+(rec?'  •  NOUVEAU RECORD':'  •  record : '+pkFmt(old)),'#7dffb0',0);
      runExplosion(player.ed,0,V3(f.x,f.y0+f.h*.5,f.z));SFX.goal();addShake(.5);
      return;
    }
  }
}
let lastCount=4;
function onGoal(team,gp){
  if(MODE.run){grGoal(gp);return;}
  if(MODE.moves){
    if(MV.state==='try'||MV.state==='demo'){mvGoal(team);runExplosion((MV.car&&MV.car.ed)||IT['expl:0'].ed,team,V3(0,GH*.45,team===0?HZ+3:-(HZ+3)));SFX.goal();}
    return;
  }
  if(MODE.train){
    if(TR.state!=='play')return;
    const ok=(cfg.pack||0)===1?false:team===0;
    trainResult(ok);SFX.goal();
    const sc0=cars.find(c=>c.team===team)||player;
    if(sc0)runExplosion(sc0.ed,team,V3(0,GH*.45,team===0?HZ+3:-(HZ+3)));
    return;
  }
  const sc=ball.last;
  const scorer=sc&&sc.team===team?sc:(cars.find(c=>c.team===team&&c.human)||cars.find(c=>c.team===team)||player);
  let gpos;
  if(MODE.hoops)gpos=V3(0,HOOP.ry,(team===0?1:-1)*(HZ-HOOP.rz));
  else if(MODE.drop)gpos=V3(ball.pos.x,1.5,ball.pos.z);
  else gpos=V3(0,GH*.45,team===0?HZ+3:-(HZ+3));
  if(gp)gpos=gp;
  lastGoal={team:team,ed:scorer?scorer.ed:IT['expl:0'].ed,pos:gpos};
  if(MODE.free){
    ball.alive=false;ball.mesh.visible=false;freeT=1.8;
    const solo=CM.cur&&CM.cur.mode==='solo';
    if(solo)scores[0]++;
    showMsg('BUT !',solo?'Buts : '+scores[0]:'',TCSS[team],1.4);runExplosion(lastGoal.ed,team,gpos);SFX.goal();addShake(.6);
    return;
  }
  scores[team]++;phase='goal';phaseT=0;ball.alive=false;ball.mesh.visible=false;lastTeamScored=team;
  goalFrame=Math.max(0,recBuf.length-1);
  let sub='';
  if(sc){
    if(sc.team===team){
      sc.stats.goals++;sc.stats.score+=100;sub=sc.name;
      if(ball.prev&&ball.prev.team===team&&ball.prev!==sc&&ball.lastT-ball.prevT<5){ball.prev.stats.assists++;ball.prev.stats.score+=50;sub+='  (passe de '+ball.prev.name+')';}
    }else sub='CSC de '+sc.name;
  }
  showMsg(MODE.hoops?'PANIER !':'BUT !',sub,TCSS[team],0);
  if(sc&&sc===player&&sc.team===team)popup('BUT',100,'#7dffb0');
  if(ball.prev&&ball.prev===player&&sc&&sc.team===team&&sc!==player)popup('PASSE DÉCISIVE',50,'#6db0ff');
  addShake(.9);crowdRoar();
  explodeLast();SFX.goal();
  if(INTRO.on)introGoal();
  if(scorer===player&&player&&!INTRO.on)playAnthem(player.L.anth);
  el.flash.style.transition='none';el.flash.style.opacity='.75';void el.flash.offsetWidth;el.flash.style.transition='opacity .8s';el.flash.style.opacity='0';
  feed('BUT pour '+TNAME[team]+' !',TCSS[team]);
}
function explodeLast(){runExplosion(lastGoal.ed,lastGoal.team,lastGoal.pos);}
function afterGoal(){
  if(overtime||(timeLeft<=0&&scores[0]!==scores[1])||(MUT.limit>0&&(scores[0]>=MUT.limit||scores[1]>=MUT.limit))){endMatch();return;}
  kickoff();
}
function endMatch(){
  phase='over';phaseT=0;clearMsg();clearPowers();
  const w=scores[0]>scores[1]?0:(scores[1]>scores[0]?1:-1),mine=player?player.team:0,win=w===mine;
  el.endT.textContent=win?'VICTOIRE !':'DÉFAITE';el.endT.style.color=win?'#7dffb0':'#ff8a8a';
  el.endS.textContent='BLEU '+scores[0]+' – '+scores[1]+' ORANGE';
  {const pool=cars.filter(c=>w<0||c.team===w).sort((x,y)=>y.stats.score-x.stats.score);if(pool[0])el.endS.textContent+='  •  ⭐ MVP : '+pool[0].name+' ('+pool[0].stats.score+')';}
  const rows=statRows();showScoreboard(false);
  el.endTable.innerHTML='<table><tr><th>JOUEUR</th><th>SCORE</th><th>BUTS</th><th>PASSES</th><th>ARRÊTS</th><th>TIRS</th><th>DÉMOS</th></tr>'+rows+'</table>';
  const rew=matchRewards(win);
  let h='<div class="rew"><span>💰 +'+rew.cr+'</span><span>⭐ +'+rew.xp+' XP</span>';
  if(rew.levelUp)h+='<span class="up">NIVEAU '+lvlOf(SV.xp)+' !</span>';
  if(rew.rank)h+='<span style="color:'+rew.rank.c+'">'+rew.rank.n+(rew.rank.un?' – placement '+(PLACE_N-rew.rank.left)+' / '+PLACE_N:' ('+(rew.mmr>0?'+':'')+rew.mmr+')')+'</span>';
  if(rew.item)h+='<span class="up">🎁 '+esc(rew.item.n)+'</span>';
  if(rew.ndDone)h+='<span class="up">New Driver terminé • rang de départ : '+esc(rankOf(SV.ndStart||520).n)+'</span>';
  rew.tier.forEach(r=>{h+='<span class="up">Passe : '+(r.type==='cr'?'+'+r.val+' 💰':esc(IT[r.val].n))+'</span>';});
  h+='</div>';
  if(MODE.tour){
    tourFinish(win);
    h+='<div class="sub">'+(SV.tour.champ?'🏆 Tournoi remporté ! +'+TOUR_PRIZE[2]+' 💰':(win?'Qualifié pour le tour suivant ! +'+TOUR_PRIZE[Math.max(0,SV.tour.round-1)]+' 💰':'Éliminé du tournoi.'))+'</div>';
    $('again').textContent='Continuer';
  }else if(cfg.lg&&!MODE.free&&!MODE.train&&!MODE.moves){
    lgFinish(win);h+='<div class="sub">'+lgSummary(win)+'</div>';$('again').textContent='Continuer';
  }else $('again').textContent='Rejouer';
  $('endRew').innerHTML=h;
  el.end.classList.add('show');el.touch.classList.add('hide');
}

/* ---------- replay ---------- */
let rIdx=0,rLen=0,rStart=0,rExploded=false;
const rCam=V3(),rDir=V3(0,0,1);
function startReplay(){
  if(recBuf.length<40){afterGoal();return;}
  phase='replay';phaseT=0;rLen=recBuf.length;rStart=Math.max(0,rLen-300);rIdx=rStart;rExploded=false;
  clearMsg();el.rtag.classList.remove('hide');ball.mesh.visible=true;
  const b=recBuf[rStart];rCam.set(b[0],b[1]+8,b[2]-16);
}
function endReplay(){
  if(phase!=='replay')return;
  el.rtag.classList.add('hide');ball.mesh.visible=false;
  afterGoal();
}
function applyReplay(){
  const i=Math.floor(rIdx),f=rIdx-i,a=recBuf[Math.min(i,rLen-1)],b=recBuf[Math.min(i+1,rLen-1)];
  ball.mesh.position.set(lerp(a[0],b[0],f),lerp(a[1],b[1],f),lerp(a[2],b[2],f));
  ball.mesh.visible=i<=goalFrame;
  let k=3;
  for(const c of cars){
    c.mesh.position.set(lerp(a[k],b[k],f),lerp(a[k+1],b[k+1],f),lerp(a[k+2],b[k+2],f));
    qa.set(lerp(a[k+3],b[k+3],f),lerp(a[k+4],b[k+4],f),lerp(a[k+5],b[k+5],f),lerp(a[k+6],b[k+6],f)).normalize();c.mesh.quaternion.copy(qa);
    c.mesh.visible=a[k+7]>=0;c.boosting=a[k+7]>0;c.dead=a[k+7]<0;
    c.pos.copy(c.mesh.position);c.fwd.set(0,0,-1).applyQuaternion(qa);
    k+=8;
  }
}

/* =====================================================================
   CAMÉRA
   ===================================================================== */
const camPos=V3(0,8,20),camLook=V3(),camHead=V3(0,0,-1),wantPos=V3(),wantLook=V3();
let fovCur=72,menuAng=0;
function clampCam(p){if(OPEN){p.y=Math.max(p.y,1.3);return;}if(DROP.on){hexFit(p,1.5-R);p.y=clamp(p.y,1.3,H-1.3);return;}p.x=clamp(p.x,-HX+1.5,HX-1.5);p.y=clamp(p.y,1.3,H-1.3);p.z=clamp(p.z,-HZ+1.5,HZ-1.5);}
function followCam(dt,c,useBall){
  if(useBall&&ball.alive){
    tA.copy(ball.pos).sub(c.pos);const d=tA.length();
    if(d>4){tA.divideScalar(d);camHead.lerp(tA,dampT(7,dt)).normalize();}
    wantPos.copy(c.pos).addScaledVector(camHead,-(SV.set.camD||11.5));wantPos.y+=(SV.set.camH||4.4)-.8+Math.max(0,camHead.y)*5;
    wantLook.copy(ball.pos).lerp(c.pos,.18);
  }else{
    const hl=Math.hypot(c.fwd.x,c.fwd.z);
    if(hl>.35){tA.set(c.fwd.x,0,c.fwd.z).divideScalar(hl);camHead.lerp(tA,dampT(5,dt)).normalize();}
    wantPos.copy(c.pos).addScaledVector(camHead,-(SV.set.camD||11.5));wantPos.y+=SV.set.camH||4.4;
    wantLook.copy(c.pos).addScaledVector(camHead,7);wantLook.y+=1.8;
  }
  clampCam(wantPos);
  camPos.lerp(wantPos,dampT(SV.set.camS||10,dt));camLook.lerp(wantLook,dampT((SV.set.camS||10)*1.2,dt));
}
function updateCamera(dt){
  let fovT=camera.aspect<1?92:(SV.set.camF||78);
  if(INTRO.on&&phase!=='menu'){
    fovT=introCam(dt);
  }else if(phase==='menu'){
    lobbyCamera(dt);fovT=camera.aspect<1?46:lobby.fov;
  }else if(phase==='replay'){
    const i=Math.floor(rIdx),a=recBuf[Math.min(i,rLen-1)],b=recBuf[Math.max(0,Math.min(i-4,rLen-1))];
    tA.set(a[0]-b[0],0,a[2]-b[2]);if(tA.length()>.05){tA.normalize();rDir.lerp(tA,dampT(3,dt)).normalize();}
    wantPos.set(a[0]-rDir.x*17,a[1]+7,a[2]-rDir.z*17);clampCam(wantPos);
    camPos.lerp(wantPos,dampT(5,dt));camLook.set(a[0],a[1],a[2]);
  }else if(phase==='goal'){
    const s=lastTeamScored===0?1:-1;
    wantPos.set(0,11,s*(HZ-30));wantLook.set(0,GH*.5,s*HZ);
    camPos.lerp(wantPos,dampT(1.6,dt));camLook.lerp(wantLook,dampT(4,dt));
  }else if(phase==='over'){
    menuAng+=dt*.25;camPos.set(Math.cos(menuAng)*40,14,Math.sin(menuAng)*40);camLook.set(0,3,0);
  }else if(player){
    const fc=(MODE.moves&&MV.car&&MV.state!=='try'&&MV.state!=='wait')?MV.car:player;
    followCam(dt,fc,MODE.moves&&MV.state==='demo'?true:(!!cfg.ballCam&&!MODE.park));
    if(fc.boosting)fovT+=7;
  }
  if(player&&phase==='play'&&player.vel.length()>=45)fovT+=4;
  fovCur=lerp(fovCur,fovT,dampT(6,dt));
  if(Math.abs(camera.fov-fovCur)>.05){camera.fov=fovCur;camera.updateProjectionMatrix();}
  camera.position.copy(camPos);camera.up.set(0,1,0);
  if(shake>.01){
    const s=shake*shake*.9;camera.position.x+=(rnd()-.5)*s;camera.position.y+=(rnd()-.5)*s;camera.position.z+=(rnd()-.5)*s;
    shake=Math.max(0,shake-dt*2.4);
  }
  camera.lookAt(camLook);
  if(INTRO.on&&phase!=='menu')camera.rotateZ(INTRO.roll); /* léger penchage de la caméra (plan « cinéma ») */
}

/* =====================================================================
   INTRO DU PREMIER LANCEMENT
   Un match 2v2 où TOUT LE MONDE est piloté par l'IA (y compris la voiture du joueur), filmé avec des plans de cinéma :
   orbite autour de la balle, caméra basse derrière une voiture, grue sur le côté, caméra derrière la cage, tour du but.
   Effets : coupe franche à chaque plan, penchage, zoom (FOV) et tremblement à chaque frappe, ralenti sur les tirs puissants,
   bandes noires de cinéma et titre. Ne compte ni dans les stats, ni dans les défis, ni dans les récompenses.
   Se termine tout seul, ou au toucher / à une touche / à un bouton de manette.
   ===================================================================== */
const INTRO={on:false,gate:false,t:0,dur:30,script:0,st:0,sc:null,atk:1,x0:0,hold:4.8,boom:false,shot:-1,shotT:0,ang:0,bv:0,punch:0,slow:0,roll:0,snap:false,car:null,n:0,save:null};
function introStart(){
  if(INTRO.on||phase!=='menu')return false;
  INTRO.save={mode:cfg.mode,mut:cfg.mut,lg:cfg.lg,size:cfg.size,dur:cfg.dur,team:cfg.team,skill:cfg.skill,diff:cfg.diff,party:cfg.party,partySk:cfg.partySk,auto:cfg.autoPilot,ballCam:cfg.ballCam,pend:CM.pend};
  cfg.mode='casual';cfg.mut=null;cfg.lg=false;cfg.size=2;cfg.dur=0;cfg.team=0;cfg.skill=.95;cfg.party=[];cfg.partySk=[];cfg.autoPilot=true;cfg.ballCam=1;CM.pend=null;
  INTRO.on=true;INTRO.t=0;INTRO.shot=-1;INTRO.shotT=0;INTRO.ang=rnd()*6.28;INTRO.bv=0;INTRO.punch=0;INTRO.slow=0;INTRO.roll=0;INTRO.n=0;INTRO.car=null;INTRO.script=0;INTRO.st=0;INTRO.sc=null;INTRO.boom=false;
  if(GATE.on){INTRO.gate=true;GATE.hide();}   /* l'écran de connexion était déjà ouvert : on le cache le temps de l'intro */
  document.body.classList.add('intro');
  startMatch();
  if(phase==='countdown')countT=.9;
  MUT.demo='off';   /* personne ne se fait démolir pendant l'intro */
  const fx=$('introFx');fx.classList.remove('in','boom');fx.classList.add('show');void fx.offsetWidth;fx.classList.add('in');
  return true;
}
function introEnd(){
  if(!INTRO.on)return;
  INTRO.on=false;
  const s=INTRO.save;
  if(s){cfg.mode=s.mode;cfg.mut=s.mut;cfg.lg=s.lg;cfg.size=s.size;cfg.dur=s.dur;cfg.team=s.team;cfg.skill=s.skill;cfg.diff=s.diff;cfg.party=s.party;cfg.partySk=s.partySk;cfg.autoPilot=s.auto;cfg.ballCam=s.ballCam;CM.pend=s.pend;}
  INTRO.save=null;MUT.speed=1;
  document.body.classList.remove('intro');
  $('introFx').classList.remove('show','in','boom');
  toMenu();
  if(INTRO.gate){INTRO.gate=false;if(!CLOUD.user)GATE.show();}
}
/* Le but de l'intro : après quelques secondes de jeu normal, un bot reçoit une balle devant lui, fonce dessus et marque à coup sûr.
   Les autres voitures continuent de jouer mais ne peuvent plus toucher la balle (noBall). L'explosion du but fait apparaître le titre. */
function introScript(){
  if(!HASGOAL||INTRO.script)return;
  const bots=cars.filter(c=>!c.human),sc=bots[(rnd()*bots.length)|0]||cars[0];
  const atk=sc.team===0?1:-1,x0=(rnd()-.5)*14;
  INTRO.sc=sc;INTRO.atk=atk;INTRO.x0=x0;INTRO.script=1;INTRO.st=0;
  for(const c of cars)if(c!==sc)c.noBall=true;
  placeCar(sc);
  sc.pos.set(x0,CR+.02,-atk*10);sc.vel.set(0,0,atk*26);sc.boost=100;
  ball.pos.set(x0,BR+.05,atk*(HZ-52));ball.vel.set(0,0,atk*8);ball.py=ball.pos.y;ball.mesh.position.copy(ball.pos);
  ball.alive=true;ball.mesh.visible=true;ball.last=null;ball.prev=null;ball.frozen=0;ball.held=null;
  const e=IT['expl:13'];if(e&&e.ed)sc.ed=e.ed;   /* Supernova : la plus grosse explosion */
}
function introShoot(){
  const sc=INTRO.sc,atk=INTRO.atk;
  ball.last=sc;ball.prev=null;ball.lastT=clockT;ball.halo.material.color.set(TEAM[sc.team]);
  const tx=INTRO.x0*.3+(rnd()-.5)*6,ty=GH*.4,tz=atk*(HZ+1.5);
  const dx=tx-ball.pos.x,dz=tz-ball.pos.z,dh=Math.hypot(dx,dz)||1,sp=Math.min(95,Math.max(68,ball.vel.length()+8)),T=dh/sp;
  ball.vel.set(dx/T,(ty-ball.pos.y)/T+G*T/2,dz/T);
  INTRO.script=2;INTRO.st=0;
}
function introGoal(){   /* appelé par onGoal : l'explosion révèle le nom du jeu */
  if(INTRO.boom)return;
  INTRO.boom=true;
  const fx=$('introFx');fx.classList.remove('boom');void fx.offsetWidth;fx.classList.add('boom');
}
function introTick(dt){
  INTRO.t+=dt;
  if(phase==='countdown'&&countT>.9)countT=.9;   /* pas de compte à rebours : on est déjà dans l'action */
  if(phase==='play'&&!INTRO.script&&INTRO.t>=4.2)introScript();
  if(INTRO.script&&phase==='play'){
    INTRO.st+=dt;
    const sc=INTRO.sc;
    if(INTRO.script===1){
      const I=sc.in;I.x=0;I.y=0;I.thr=1;I.boost=true;I.jump=false;I.drift=false;I.rollMode=false;I.roll=0;
      if(ball.last===sc||INTRO.st>5.5)introShoot();
    }
  }
  if(INTRO.t>=INTRO.dur)introEnd();
}
function introCam(dt){
  const focus=phase==='goal'?lastGoal.pos:ball.mesh.position;
  const want=phase==='goal'?9:INTRO.script===1?4:INTRO.script===2?5:Math.floor(INTRO.t/4.5)%4;
  if(want!==INTRO.shot){INTRO.shot=want;INTRO.shotT=0;INTRO.snap=true;INTRO.n++;INTRO.car=cars.length?cars[INTRO.n%cars.length]:null;INTRO.punch=Math.max(INTRO.punch,.55);}
  INTRO.shotT+=dt;INTRO.ang+=dt*(want===9?1.1:.45);
  const c=INTRO.car,st=INTRO.shotT;let rate=4,fov=66,roll=0;
  if(want===0){        /* orbite autour de la balle, qui se resserre */
    const Rr=Math.max(14,24-st*1.6);
    wantPos.set(focus.x+Math.cos(INTRO.ang)*Rr,focus.y+6+Math.sin(st*.8)*1.5,focus.z+Math.sin(INTRO.ang)*Rr);wantLook.copy(focus);
    fov=64;rate=5;roll=Math.sin(INTRO.t*.6)*.05;
  }else if(want===1&&c){ /* caméra basse derrière une voiture, grand angle */
    const hl=Math.hypot(c.fwd.x,c.fwd.z)||1;tA.set(c.fwd.x/hl,0,c.fwd.z/hl);
    wantPos.copy(c.pos).addScaledVector(tA,-9);wantPos.y=c.pos.y+2.4;
    wantLook.copy(c.pos).addScaledVector(tA,12).lerp(focus,.5);
    fov=86+(c.boosting?10:0);rate=7;roll=Math.sin(INTRO.t*1.3)*.07;
    if(c.boosting)addShake(.18);
  }else if(want===2){  /* grue sur le côté, téléobjectif qui s'élargit */
    const sx=INTRO.n%2?1:-1;
    wantPos.set(sx*(HX-3),H*.5+Math.sin(st)*1.5,focus.z*.55);wantLook.copy(focus);
    fov=50+st*2.5;rate=2.5;
  }else if(want===3){  /* caméra au ras du sol, devant la cage, qui regarde le jeu */
    const s=focus.z>=0?1:-1;
    wantPos.set(focus.x*.3,3.5+st*.4,s*(HZ-3));wantLook.copy(focus);
    fov=72;rate=3;roll=Math.sin(INTRO.t*.9)*.04;
  }else if(want===4&&INTRO.sc){  /* le buteur fonce vers la balle : caméra derrière lui */
    const sc=INTRO.sc;tA.set(0,0,INTRO.atk);
    wantPos.copy(sc.pos).addScaledVector(tA,-10);wantPos.y=sc.pos.y+3.2;wantPos.x+=INTRO.x0>=0?-4:4;
    wantLook.copy(sc.pos).addScaledVector(tA,14).lerp(ball.pos,.55);
    fov=80+(sc.boosting?10:0);rate=6;roll=Math.sin(INTRO.t*1.5)*.05;
    if(sc.boosting)addShake(.15);
  }else if(want===5){  /* la frappe : caméra basse sur le côté de la cage, la balle file dedans */
    wantPos.set((INTRO.x0>=0?-1:1)*20,4.5,INTRO.atk*(HZ-18));wantLook.copy(focus);
    fov=60;rate=4.5;roll=0;
  }else{               /* but marqué : tour rapide autour de l'explosion */
    wantPos.set(focus.x+Math.cos(INTRO.ang)*13,focus.y+5,focus.z+Math.sin(INTRO.ang)*13);wantLook.copy(focus);
    fov=58;rate=3;
  }
  clampCam(wantPos);
  if(INTRO.snap){camPos.copy(wantPos);camLook.copy(wantLook);INTRO.snap=false;}   /* coupe franche */
  else{camPos.lerp(wantPos,dampT(rate,dt));camLook.lerp(wantLook,dampT(rate*1.4,dt));}
  /* frappe de balle : tremblement + coup de zoom, et ralenti si la frappe est très puissante */
  const bv=ball.alive?ball.vel.length():0;
  if(bv-INTRO.bv>16){const k=Math.min(1,(bv-INTRO.bv)/50);addShake(.25+.45*k);INTRO.punch=Math.max(INTRO.punch,.4+.6*k);if(bv>52&&INTRO.slow<=0)INTRO.slow=.55;}
  INTRO.bv=bv;INTRO.punch=Math.max(0,INTRO.punch-dt*2.2);
  if(INTRO.slow>0){INTRO.slow-=dt;MUT.speed=INTRO.slow>0?.35:1;}else MUT.speed=1;
  INTRO.roll=lerp(INTRO.roll,roll,dampT(4,dt));
  return fov+INTRO.punch*9+(camera.aspect<1?20:0);
}
/* sortie : toucher, touche du clavier (après un court instant pour ne pas sauter la démo par erreur) */
document.addEventListener('pointerdown',()=>{if(INTRO.on&&INTRO.t>.6)introEnd();});
document.addEventListener('keydown',()=>{if(INTRO.on&&INTRO.t>.6)introEnd();});

/* =====================================================================
   ENTRÉES
   ===================================================================== */
const keys={},btn={jump:false,boost:false,drift:false,power:false},joy={id:-1,ox:0,oy:0,x:0,y:0};
let prevJump=false,prevPw=false;
function readPlayerInput(){
  const c=player,I=c.in;
  let x=joy.x,y=joy.y;
  const dz=v=>Math.abs(v)<.1?0:v;x=dz(x);y=dz(y);
  const kx=(keys.KeyD||keys.ArrowRight?1:0)-(keys.KeyA||keys.ArrowLeft?1:0),ky=(keys.KeyS||keys.ArrowDown?1:0)-(keys.KeyW||keys.ArrowUp?1:0);
  const usingKeys=(kx!==0||ky!==0);
  if(usingKeys){x=kx;y=ky;}
  const gp=GP.in,usingPad=!usingKeys&&!!gp&&gp.act; /* manette : stick = direction / tangage, gâchettes = gaz */
  if(usingPad){x=gp.x;y=gp.y;}
  x=clamp(x,-1,1);y=clamp(y,-1,1);
  I.x=x;I.y=y;
  if(usingPad)I.thr=gp.thr;
  else if(!usingKeys&&isTouch&&cfg.autoThr)I.thr=y>.4?-Math.min(1,(y-.15)*1.3):1;
  else I.thr=-y;
  I.boost=!!(keys.ShiftLeft||keys.ShiftRight||btn.boost||(gp&&gp.boost));
  const j=!!(keys.Space||btn.jump||(gp&&gp.jump));
  if(j&&!prevJump)I.jumpEdge=true;
  prevJump=j;I.jump=j;
  I.drift=!!(keys.ControlLeft||keys.ControlRight||keys.KeyX||btn.drift||(gp&&gp.drift));I.rollMode=I.drift;
  I.roll=(keys.KeyE?1:0)-(keys.KeyQ?1:0)||(gp?gp.roll:0);
  const pf=!!(keys.KeyF||btn.power||(gp&&gp.power));if(pf&&!prevPw)I.pwEdge=true;prevPw=pf;
}
window.addEventListener('keydown',e=>{
  /* on tape dans un champ (pseudo, réglages) : pas de raccourcis de jeu */
  if(e.target&&e.target.tagName&&/^(INPUT|TEXTAREA|SELECT)$/.test(e.target.tagName))return;
  keys[e.code]=true;
  if(['Space','ArrowUp','ArrowDown','ArrowLeft','ArrowRight','ShiftLeft','ControlLeft'].indexOf(e.code)>=0&&phase!=='menu')e.preventDefault();
  if(e.repeat)return;
  if(e.code==='KeyC')cfg.ballCam=cfg.ballCam?0:1;
  if(e.code==='KeyR'&&phase==='play'&&player){if(MODE.free){if(MODE.run)grSpawn();else{resetBall();placeCar(player);if(MODE.park)parkReset();}}else if(MODE.train)trainSetup();else if(MODE.moves&&(MV.state==='try'||MV.state==='wait'))mvBeginTry(false);}
  if(MODE.moves&&phase==='play'){
    if((e.code==='Enter'||e.code==='NumpadEnter')&&(MV.state==='demo'||MV.state==='demoEnd'))mvSkip();
    else if(e.code==='KeyV')mvReplay();
    else if(e.code==='KeyN')mvLoad(MV.i+1);
    else if(e.code==='KeyB')mvLoad(MV.i-1);
  }
  if(e.code==='Tab'){e.preventDefault();showScoreboard(true);}
  if(e.code==='Escape'||e.code==='KeyP')togglePause();
  if(phase==='replay'&&(e.code==='Space'||e.code==='Enter'))endReplay();
});
window.addEventListener('keyup',e=>{keys[e.code]=false;if(e.code==='Tab')showScoreboard(false);});
window.addEventListener('blur',()=>{for(const k in keys)keys[k]=false;showScoreboard(false);if(phase==='play'||phase==='countdown')togglePause(true);});
canvas.addEventListener('pointerdown',e=>{
  if(phase==='replay'){endReplay();return;}
  if(e.pointerType==='mouse'&&e.button===0&&(phase==='play'))btn.boost=true;
  if(e.pointerType==='mouse'&&e.button===2&&(phase==='play'))btn.power=true;
});
window.addEventListener('pointerup',e=>{if(e.pointerType==='mouse'){btn.boost=false;btn.power=false;}});
document.addEventListener('contextmenu',e=>e.preventDefault());

const joyZone=$('joyZone'),joyBase=$('joyBase'),joyKnob=$('joyKnob');
joyZone.addEventListener('pointerdown',e=>{
  if(phase==='replay'){endReplay();return;}
  if(joy.id!==-1)return;joy.id=e.pointerId;joy.ox=e.clientX;joy.oy=e.clientY;joy.x=joy.y=0;
  joyBase.style.display='block';joyBase.style.left=e.clientX+'px';joyBase.style.top=e.clientY+'px';joyKnob.style.transform='translate(0,0)';
  try{joyZone.setPointerCapture(e.pointerId);}catch(_){}
  e.preventDefault();
});
joyZone.addEventListener('pointermove',e=>{
  if(e.pointerId!==joy.id)return;
  const Rr=Math.min(window.innerWidth,window.innerHeight)*.11;let dx=e.clientX-joy.ox,dy=e.clientY-joy.oy;
  const l=Math.hypot(dx,dy),k=l>Rr?Rr/l:1;dx*=k;dy*=k;
  joy.x=dx/Rr;joy.y=dy/Rr;joyKnob.style.transform='translate('+dx+'px,'+dy+'px)';
});
function joyEnd(e){if(e.pointerId!==joy.id)return;joy.id=-1;joy.x=joy.y=0;joyBase.style.display='none';}
joyZone.addEventListener('pointerup',joyEnd);joyZone.addEventListener('pointercancel',joyEnd);
function bindBtn(id,key){
  const b=$(id);
  b.addEventListener('pointerdown',e=>{btn[key]=true;b.classList.add('down');try{b.setPointerCapture(e.pointerId);}catch(_){}e.preventDefault();});
  const up=()=>{btn[key]=false;b.classList.remove('down');};
  b.addEventListener('pointerup',up);b.addEventListener('pointercancel',up);b.addEventListener('lostpointercapture',up);
}
bindBtn('bJump','jump');bindBtn('bBoost','boost');bindBtn('bDrift','drift');bindBtn('bPower','power');
function mvMinimize(on){
  MV.min=on===undefined?!MV.min:!!on;
  $('mv').classList.toggle('min',MV.min);
  $('mvMin').textContent=MV.min?'+':'–';
  try{localStorage.setItem('mvMin',MV.min?'1':'0');}catch(_){}
}
try{MV.min=localStorage.getItem('mvMin')==='1';}catch(_){MV.min=false;}
mvMinimize(MV.min);
$('mvMin').addEventListener('pointerdown',e=>{e.preventDefault();e.stopPropagation();mvMinimize();});
$('mvB').addEventListener('pointerdown',e=>{
  const b=e.target.closest('[data-mv]');if(!b||!MODE.moves)return;e.preventDefault();e.stopPropagation();
  const a=b.dataset.mv;
  if(a==='demo')mvReplay();else if(a==='skip')mvSkip();else if(a==='next')mvLoad(MV.i+1);else if(a==='prev')mvLoad(MV.i-1);
  else if(a==='reset'&&(MV.state==='try'||MV.state==='wait'))mvBeginTry(false);
});
$('bCam').addEventListener('pointerdown',e=>{cfg.ballCam=cfg.ballCam?0:1;e.preventDefault();});
$('bPause').addEventListener('pointerdown',e=>{togglePause();e.preventDefault();});

/* =====================================================================
   MANETTE (Gamepad API) : conduite + navigation dans les menus
   Match : stick gauche = direction / tangage · RT = accélérer · LT = freiner/reculer · A = saut · B = boost
           X = dérapage / roulis · LB/RB = roulis gauche/droite · Y = caméra · L3 = remise (entraînement)
           R3 = pouvoir (Rumble) · Select (maintenu) = scores · Start = pause
   Menus : croix ou stick = déplacer · A = valider · B = retour · LB/RB = changer d'onglet · stick droit = défiler
   ===================================================================== */
const GP={on:false,in:null,prev:{},focus:null,fx:0,fy:0,hold:'',holdT:0,rep:0};
function gpGet(){const l=navigator.getGamepads?navigator.getGamepads():[];for(let i=0;i<l.length;i++){const p=l[i];if(p&&p.connected)return p;}return null;}
function gpAxis(p){const x=p.axes[0]||0,y=p.axes[1]||0,m=Math.hypot(x,y);if(m<.18)return[0,0];const k=Math.min(1,(m-.18)/.82)/m;return[x*k,y*k];}
function gpBtn(p,i){const b=p.buttons[i];return !!b&&(b.pressed||b.value>.5);}
function gpVal(p,i){const b=p.buttons[i];return b?b.value:0;}
function gpSetFocus(e){
  if(GP.focus)GP.focus.classList.remove('gpf');
  GP.focus=e;if(!e)return;
  e.classList.add('gpf');
  try{e.scrollIntoView({block:'nearest',inline:'nearest'});}catch(_){}
  const r=e.getBoundingClientRect();GP.fx=r.left+r.width/2;GP.fy=r.top+r.height/2;
}
/* boutons tactiles masqués tant qu'on joue à la manette ; ils reviennent si on touche l'écran ou si la manette est débranchée */
const gpHud=on=>document.body.classList.toggle('gp-on',!!on);
window.addEventListener('pointerdown',e=>{gpSetFocus(null);if(e.pointerType==='touch'||e.pointerType==='pen')gpHud(false);},true);
window.addEventListener('gamepadconnected',()=>{gpHud(true);toast('🎮 Manette connectée');});
window.addEventListener('gamepaddisconnected',()=>{GP.in=null;GP.on=false;gpSetFocus(null);gpHud(false);toast('Manette déconnectée');});
function gpRoot(){
  if($('modal').classList.contains('show'))return $('modal');
  if($('queue').classList.contains('show'))return $('queue');
  if(phase==='paused')return $('pause');
  if(phase==='over'&&el.end.classList.contains('show'))return el.end;
  if(phase==='menu'&&$('ui').classList.contains('show'))return $('ui');
  return null;
}
const GP_SEL='button,[data-acc],[data-lb],[data-go],[data-card],[data-mode],[data-opt],[data-mut],[data-a],[data-id],[data-tab],[data-pre],[data-col],[data-shop],[data-cr],[data-cf],[data-crate],[data-bund],[data-ch],[data-ctab],[data-multi],input,select,#chw';
function gpCands(root){
  const out=[],W=innerWidth,H=innerHeight;
  root.querySelectorAll(GP_SEL).forEach(e=>{
    if(e.disabled)return;
    const r=e.getBoundingClientRect();if(r.width<6||r.height<6)return;
    if(r.bottom<0||r.top>H||r.right<0||r.left>W)return;
    const cs=getComputedStyle(e);if(cs.visibility==='hidden'||cs.pointerEvents==='none')return;
    const t=document.elementFromPoint(Math.min(W-2,Math.max(2,r.left+r.width/2)),Math.min(H-2,Math.max(2,r.top+r.height/2)));
    if(!t||!(e.contains(t)||t.contains(e)))return;
    out.push({e:e,cx:r.left+r.width/2,cy:r.top+r.height/2});
  });
  return out;
}
function gpScroller(){
  let n=GP.focus;
  while(n&&n!==document.body){if(n.scrollHeight>n.clientHeight+4&&/(auto|scroll)/.test(getComputedStyle(n).overflowY))return n;n=n.parentElement;}
  const root=gpRoot();if(!root)return null;
  const l=root.querySelectorAll('.pbody,.cpanel,.panel,#mbox,#qbox');
  for(const s of l){if(s.scrollHeight>s.clientHeight+4)return s;}
  return null;
}
function gpMove(dir){
  const root=gpRoot();if(!root)return;
  const cs=gpCands(root);if(!cs.length)return;
  let ent=GP.focus&&cs.find(c=>c.e===GP.focus);
  if(!ent){ /* pas (ou plus) d'élément sélectionné : on en choisit un, sans bouger */
    let best=null,bs=1e9;
    if(GP.fx||GP.fy){for(const c of cs){const d=Math.hypot(c.cx-GP.fx,c.cy-GP.fy);if(d<bs){bs=d;best=c;}}}
    else{
      best=cs.find(c=>c.e.matches('.nb.on,.mode.on,.opt.on'))||null;
      if(!best)for(const c of cs){const s=c.cy*3+c.cx;if(s<bs){bs=s;best=c;}}
    }
    gpSetFocus(best.e);return;
  }
  const f=ent.e;
  if(f.tagName==='INPUT'&&f.type==='range'&&(dir==='l'||dir==='r')){
    const mn=+f.min||0,mx=f.max===''?100:+f.max,st=+f.step||1,d=Math.max(st,(mx-mn)/20)*(dir==='r'?1:-1);
    f.value=Math.min(mx,Math.max(mn,+f.value+d));
    f.dispatchEvent(new Event('input',{bubbles:true}));f.dispatchEvent(new Event('change',{bubbles:true}));return;
  }
  let best=null,bs=1e9;
  for(const c of cs){
    if(c===ent)continue;
    const dx=c.cx-ent.cx,dy=c.cy-ent.cy;let a,b;
    if(dir==='r'){if(dx<=4)continue;a=dx;b=Math.abs(dy);}
    else if(dir==='l'){if(dx>=-4)continue;a=-dx;b=Math.abs(dy);}
    else if(dir==='d'){if(dy<=4)continue;a=dy;b=Math.abs(dx);}
    else{if(dy>=-4)continue;a=-dy;b=Math.abs(dx);}
    const s=a+b*2.2;if(s<bs){bs=s;best=c;}
  }
  if(best)gpSetFocus(best.e);
  else if(dir==='d'||dir==='u'){const sc=gpScroller();if(sc)sc.scrollBy(0,dir==='d'?170:-170);}
}
function gpActivate(){
  const f=GP.focus;if(!f||!document.contains(f)){gpMove('d');return;}
  if(f.tagName==='INPUT'){if(f.type!=='range')f.focus();return;}
  f.click();
}
function gpBack(){
  if($('modal').classList.contains('show')){const c=$('modal').querySelector('[data-a="mclose"],#qn');if(c)c.click();else modalClose();return;}
  if($('queue').classList.contains('show')){const c=$('qc');if(c)c.click();return;}
  if(phase==='paused'){togglePause();return;}
  if(phase==='menu'&&!UI.home)goHome();
}
function gpTab(root,d){
  const tabs=Array.from(root.querySelectorAll('.tab')).filter(t=>t.offsetParent);if(tabs.length<2)return;
  let i=tabs.findIndex(t=>t.classList.contains('on'));i=(i<0?0:i+d+tabs.length)%tabs.length;
  tabs[i].click();
}
function gpMenuTick(p,dt,edge){
  const root=gpRoot();
  if(!root){gpSetFocus(null);return;}
  const [ax,ay]=gpAxis(p);
  let dir=gpBtn(p,12)?'u':gpBtn(p,13)?'d':gpBtn(p,14)?'l':gpBtn(p,15)?'r':'';
  if(!dir&&Math.hypot(ax,ay)>.55)dir=Math.abs(ax)>Math.abs(ay)?(ax>0?'r':'l'):(ay>0?'d':'u');
  if(dir!==GP.hold){GP.hold=dir;GP.holdT=0;GP.rep=0;if(dir)gpMove(dir);}
  else if(dir){GP.holdT+=dt;if(GP.holdT>.38){GP.rep+=dt;if(GP.rep>.13){GP.rep=0;gpMove(dir);}}}
  const ry=p.axes[3]||0;if(Math.abs(ry)>.3){const sc=gpScroller();if(sc)sc.scrollBy(0,ry*16);}
  if(edge('A',gpBtn(p,0)))gpActivate();
  if(edge('B',gpBtn(p,1)))gpBack();
  if(edge('mRB',gpBtn(p,5)))gpTab(root,1);
  if(edge('mLB',gpBtn(p,4)))gpTab(root,-1);
  if(phase==='paused'&&edge('St',gpBtn(p,9)))togglePause();
}
function gpGameTick(p,dt,edge){
  const [ax,ay]=gpAxis(p),rt=gpVal(p,7),lt=gpVal(p,6);
  GP.in={x:ax,y:ay,thr:clamp(rt-lt,-1,1),jump:gpBtn(p,0),boost:gpBtn(p,1),drift:gpBtn(p,2),roll:(gpBtn(p,5)?1:0)-(gpBtn(p,4)?1:0),power:gpBtn(p,11),act:Math.hypot(ax,ay)>0||rt>.05||lt>.05};
  if(edge('gY',gpBtn(p,3)))cfg.ballCam=cfg.ballCam?0:1;
  if(edge('St',gpBtn(p,9)))togglePause();
  const sel=gpBtn(p,8);if(edge('gSel',sel)||(!sel&&GP.prev.gSelOn)){showScoreboard(sel);}GP.prev.gSelOn=sel;
  if(phase==='replay'&&edge('A',gpBtn(p,0)))endReplay();
  if(edge('gL3',gpBtn(p,10))&&phase==='play'&&player){if(MODE.free){if(MODE.run)grSpawn();else{resetBall();placeCar(player);if(MODE.park)parkReset();}}else if(MODE.train)trainSetup();else if(MODE.moves&&(MV.state==='try'||MV.state==='wait'))mvBeginTry(false);}
}
function padTick(dt){
  const p=gpGet();GP.on=!!p;
  if(!p){GP.in=null;return;}
  if(INTRO.on&&INTRO.t>.6&&p.buttons.some(b=>b.pressed)){introEnd();return;}   /* un bouton de manette passe la démo */
  if(p.buttons.some(b=>b.pressed)||p.axes.some(v=>Math.abs(v)>.5))gpHud(true);
  const edge=(k,v)=>{const e=v&&!GP.prev[k];GP.prev[k]=v;return e;};
  if(phase==='menu'||phase==='paused'||phase==='over'||gpRoot()){GP.in=null;gpMenuTick(p,dt,edge);}
  else{gpSetFocus(null);gpGameTick(p,dt,edge);}
}

let prePause='play';
/* réinitialiser le tir : mêmes effets que la touche R (entraînement libre, parcours, packs, mécaniques) */
function canResetShot(){return !!player&&(MODE.free||MODE.train||(MODE.moves&&(MV.state==='try'||MV.state==='wait')));}
function resetShot(){
  if(!canResetShot())return;
  if(MODE.free){if(MODE.run)grSpawn();else{resetBall();placeCar(player);if(MODE.park)parkReset();}}
  else if(MODE.train)trainSetup();
  else if(MODE.moves)mvBeginTry(false);
}
function togglePause(forceOn){
  if(phase==='paused'){if(forceOn)return;phase=prePause;el.pause.classList.remove('show');return;}
  if(phase==='play'||phase==='countdown'||phase==='goal'||phase==='replay'){prePause=phase;phase='paused';$('resetShot').style.display=canResetShot()?'':'none';el.pause.classList.add('show');}
}
$('resetShot').addEventListener('click',()=>{if(phase!=='paused')return;phase=prePause;el.pause.classList.remove('show');resetShot();});
$('resume').addEventListener('click',()=>togglePause());
$('quit').addEventListener('click',()=>{el.pause.classList.remove('show');toMenu();});
$('again').addEventListener('click',()=>{
  if(MODE.tour){toMenu();go('tour');}else if(cfg.lg&&!MODE.free&&!MODE.train&&!MODE.moves){toMenu();go('season');}else{if(CM.cur)CM.pend=CM.cur;startMatch();}
});
$('toMenu').addEventListener('click',()=>{toMenu();});
function toMenu(){
  phase='menu';GR.on=false;clearPowers();clearCars();clearMsg();resetBall();
  el.hud.classList.add('hide');el.touch.classList.add('hide');el.end.classList.remove('show');el.pause.classList.remove('show');el.rtag.classList.add('hide');
  const pw=$('pw');if(pw)pw.style.display='none';
  showUI();curtain(650);
  go(UI.scr==='tour'&&!(SV.tour&&SV.tour.alive)?'lobby':(UI.scr||'lobby'));
}

/* =====================================================================
   BOUCLE PRINCIPALE
   ===================================================================== */
const SD=1/120;let acc=0,lastT=performance.now(),roleT=0;
function updatePads(dt){
  const canB=MUT.boost==='norm'||MUT.boost==='rech';
  for(const p of pads){
    if(!p.active){
      p.t-=dt;
      p.d.material.opacity=.15+.15*(1-Math.max(0,p.t)/(p.big?10:4));
      if(p.orb)p.orb.visible=false;
      if(p.t<=0){p.active=true;p.g.visible=true;p.d.material.opacity=1;if(p.orb)p.orb.visible=true;}
      continue;
    }
    if(p.orb){p.orb.rotation.y+=dt*2;p.orb.position.y=2.2+Math.sin(performance.now()/400+p.x)*.35;}
    if(phase!=='play')continue;
    for(const c of cars){
      if(c.dead||c.ghost)continue;
      const dx=c.pos.x-p.x,dz=c.pos.z-p.z,r=p.big?4.9:3.4;
      if(dx*dx+dz*dz<r*r&&c.pos.y<3.2){
        const needB=canB&&c.boost<100,needP=MODE.rumble&&!c.pw&&c.pwCool<=0;
        if(!needB&&!needP)continue;
        if(needB)c.boost=Math.min(100,c.boost+(p.big?100:12));
        if(needP)givePower(c);
        p.active=false;p.t=p.big?10:4;p.g.visible=true;p.d.material.opacity=.15;if(p.orb)p.orb.visible=false;
        if(c===player){SFX.pad(p.big);if(p.big&&needB)popup('BOOST MAX',0,'#ffb02e');}
        burst(tD.set(p.x,.8,p.z),0xffc266,p.big?24:8,p.big?14:8,.5);break;
      }
    }
  }
}
function updateVisuals(dt){
  for(const c of cars){
    if(phase!=='replay'){c.mesh.position.copy(c.pos);c.mesh.quaternion.copy(c.q);}
    const ud=c.mesh.userData,on=c.boosting&&c.mesh.visible;
    ud.flame.visible=on;ud.flame2.visible=on;
    if(on){
      const f=.75+rnd()*.5;ud.flame.scale.set(1,1,f*(c.grounded?1:1.2));ud.flame2.scale.set(1,1,f);
      if(phase!=='replay'||rnd()<.5)boostEmit(c,c.vel.length()>=40);
    }
    if(phase==='play'&&c.mesh.visible)trailEmit(c);
    spinTick(c.mesh,dt);
    if(ud.wheels&&c.mesh.visible&&phase!=='replay'){
      const fsp=c.grounded?c.vel.dot(c.fwd):0;ud.spinA+=fsp*dt/(ud.wr||.55);
      const steer=c.grounded?-c.in.x*.45:0;
      for(const w of ud.wheels){w.spin.rotation.x=ud.spinA;if(w.front)w.piv.rotation.y=lerp(w.piv.rotation.y,steer,dampT(14,dt));}
    }
    if(phase==='play'&&c.mesh.visible&&c.grounded){
      const spd=c.vel.length(),side=Math.abs(c.vel.dot(c.right));
      if((side>7||(c.in.drift&&spd>12))&&c.gn.y>.9){
        const yaw=Math.atan2(-c.fwd.x,-c.fwd.z);
        for(const sg of [-1,1]){
          tA.copy(c.pos).addScaledVector(c.fwd,1.35).addScaledVector(c.right,sg*.95);
          addSkid(tA.x,tA.z,yaw);
          if(rnd()<.6)emit(tA.x,.3,tA.z,(rnd()-.5)*3,1.5+rnd()*2,(rnd()-.5)*3,.45,.45,.5,.5);
        }
      }
      if(spd>=45&&rnd()<.8){
        tA.copy(c.pos).addScaledVector(c.fwd,-2.2);
        emit(tA.x,tA.y,tA.z,0,0,0,.35,.6,1,.45);
      }
    }
    const fr=OPEN?0:R,flat=inFlat(c.mesh.position.x,c.mesh.position.z,fr)&&c.mesh.visible;
    c.shadow.visible=flat;
    if(flat){const h=c.mesh.position.y;c.shadow.position.set(c.mesh.position.x,.05,c.mesh.position.z);const s=clamp(2.4-h*.06,1,2.4);c.shadow.scale.set(s,s*1.4,1);c.shadow.rotation.z=Math.atan2(c.fwd.x,c.fwd.z);c.shadow.material.opacity=clamp(.45-h*.018,.08,.45);}
  }
  // balle
  if(phase!=='replay')ball.mesh.position.copy(ball.pos);
  if(ball.mesh.visible){
    const vx=ball.vel.x,vz=ball.vel.z,sp=Math.hypot(vx,vz);
    if(ball.puck){ball.mesh.rotation.y+=Math.min(6,sp*.25)*dt;}
    else if(sp>.1){tA.set(vz,0,-vx).normalize();qa.setFromAxisAngle(tA,sp*dt/BR);ball.mesh.quaternion.premultiply(qa);}
    const bsp=ball.vel.length();
    if(!ball.puck){
      const mat=ball.sphere.material;
      if(MODE.drop){mat.emissive.setHex(DROP.charge>0?(DROP.team===0?0x1b5fc8:0xd0601b):0x3a5a88);mat.emissiveIntensity=.6+.35*DROP.charge;
        ball.halo.material.color.set(dropBallColor());ball.halo.scale.setScalar(13+DROP.charge*4.5);ball.halo.material.opacity=DROP.charge>0?.55+.12*DROP.charge:.35;}
      else if(MODE.heat&&HS.team>=0){mat.emissive.setHex(HS.team===0?0x1b5fc8:0xd0601b);mat.emissiveIntensity=.9;}
      else{mat.emissive.setHex(0x3a5a88);mat.emissiveIntensity=1;}
      if(!MODE.drop){ball.halo.scale.set(13,13,1);ball.halo.material.opacity=.5;}
    }
    if(phase==='play'&&bsp>38){
      const col=ball.last?TEAM[ball.last.team]:0xffffff,k=Math.min(1,(bsp-38)/30);
      const r=((col>>16)&255)/255,g=((col>>8)&255)/255,b=(col&255)/255;
      for(let i=0;i<2;i++)emit(ball.pos.x-ball.vel.x*dt*i*.5,ball.pos.y-ball.vel.y*dt*i*.5,ball.pos.z-ball.vel.z*dt*i*.5,(rnd()-.5)*2,(rnd()-.5)*2,(rnd()-.5)*2,r*(.6+k*.4),g*(.6+k*.4),b*(.6+k*.4),.45+k*.3);
    }
    const bp=ball.mesh.position,flat=inFlat(bp.x,bp.z,R)&&!(MODE.drop&&bp.y<0);
    ballShadow.visible=flat;if(flat){ballShadow.position.set(bp.x,.05,bp.z);const s=clamp(2.9-bp.y*.08,1.2,2.9)*(BR/2.6);ballShadow.scale.set(s,s,1);ballShadow.material.opacity=clamp(.5-bp.y*.02,.1,.5);}
    /* le palet est plat (demi-hauteur ≈ 1) alors que la collision est une sphère : on le pose visuellement sur la glace */
    if(ball.puck)bp.y-=BR*.615;
  }else ballShadow.visible=false;
}
function modeUpdate(dt){
  if(phase==='play'||phase==='goal')powerWorld(dt);
  powerBeams();powerHUD();
  if(MODE.train&&phase==='play')trainUpdate(dt);
  if(MODE.moves)mvUpdate(dt);
  rsChipUpdate();
  if(MODE.run)runUpdate(dt);
  if(MODE.free&&!ball.alive&&phase==='play'&&!MODE.park&&!MODE.run){freeT-=dt;if(freeT<=0){resetBall();}}
  if(MODE.park&&phase==='play')parkUpdate(dt);
}
function update(dt){
  if(INTRO.on)introTick(dt);
  phaseT+=dt;
  if(msgT>0){msgT-=dt;if(msgT<=0)clearMsg();}
  if(phase==='menu'){
    lobbyUpdate(dt);
  }else if(phase==='countdown'){
    countT-=dt;
    const n=Math.ceil(countT);
    if(n!==lastCount&&n>=1&&n<=3){lastCount=n;showMsg(String(n),'',n===1?'#ffd166':'#fff',1.2);SFX.beep(false);}
    if(countT<=0){phase='play';phaseT=0;showMsg('GO !','','#7dffb0',.8);SFX.beep(true);}
  }else if(phase==='play'||phase==='goal'){
    if(phase==='play'){
      if(!cfg.autoPilot)readPlayerInput();
      roleT-=dt;if(roleT<=0){roleT=.3;assignRoles();}
      for(const c of cars)if((!c.human||cfg.autoPilot)&&!c.coach&&!(INTRO.on&&INTRO.script&&c===INTRO.sc))botThink(c,dt);
      if(!unlimited){
        if(overtime)otTime+=dt;
        else{timeLeft-=dt;if(timeLeft<=0){timeLeft=0;if(scores[0]===scores[1]){overtime=true;otTime=0;showMsg('PROLONGATION','But en or !','#ffd166',2.2);}}}
        if(!overtime&&timeLeft<=0&&scores[0]!==scores[1]&&(ball.pos.y<4||MODE.hoops||MODE.drop)){endMatch();}
      }
    }else{
      for(const c of cars){const I=c.in;I.x=I.y=I.thr=0;I.boost=I.jump=I.drift=I.rollMode=false;}
      if(INTRO.on){if(phaseT>INTRO.hold)introEnd();}else if(phaseT>3.6)startReplay();   /* intro : le titre reste affiché, puis direction le menu */
    }
    if(phase==='play'||phase==='goal'){
      acc+=(phase==='goal'?dt*(phaseT<1.2?.3:phaseT<1.8?lerp(.3,1,(phaseT-1.2)/.6):1):dt)*MUT.speed;let n=0;
      while(acc>=SD&&n<8){simStep(SD);acc-=SD;n++;}
      if(n>=8)acc=0;
      updatePads(dt);
    }
  }else if(phase==='replay'){
    rIdx+=dt*60*.7;
    if(!rExploded&&rIdx>=goalFrame){rExploded=true;explodeLast();}
    if(rIdx>=rLen-1)endReplay();else applyReplay();
  }else if(phase==='over'){
    acc+=dt;let n=0;while(acc>=SD&&n<4){for(const c of cars){const I=c.in;I.x=I.y=I.thr=0;I.boost=I.jump=false;}simStep(SD);acc-=SD;n++;}
  }
  if(phase!=='menu')modeUpdate(dt);
  updParticles(dt);updRings(dt);
  updateStadium(dt);
  if(phase!=='menu')updateVisuals(dt);
  updateCamera(dt);
  if(phase!=='menu')updateHUD();
  updateAudio();musTick();saverTick();
}
function frame(now){
  requestAnimationFrame(frame);
  let dt=Math.min(.05,(now-lastT)/1000);lastT=now;if(!(dt>0))dt=1/60;
  try{padTick(dt);}catch(_){}
  if(phase!=='paused')update(dt);
  if(!SAVER.on)renderer.render(phase==='menu'?lobbyScene:scene,camera);   /* écran de veille : on ne dessine plus la 3D */
}

/* =====================================================================
   INIT
   ===================================================================== */
cfg.ballCam=SV.set.ballCam;cfg.autoThr=SV.set.autoThr;
{const mb=$('music');if(mb)mb.onclick=()=>{if(MUS.on){musNext();toast('♪ '+MUS.name);}};}
wallLoad();
toMenu();
requestAnimationFrame(frame);
/* PREMIER LANCEMENT : dès qu'un joueur ouvre le jeu pour la toute première fois (avant même la connexion), l'intro se joue.
   Le drapeau est gardé dans le navigateur, à part de la sauvegarde. Un joueur qui a déjà de la progression n'est pas un nouveau joueur : son drapeau est posé sans intro. */
setTimeout(()=>{
  const K='rocketArena3D.introSeen';let seen=false;
  try{seen=!!localStorage.getItem(K);}catch(e){}
  if(seen)return;
  try{localStorage.setItem(K,'1');}catch(e){}
  if(CLOUD.pristine(SV))introStart();
},400);
window.__rl={cfg:cfg,startMatch:startMatch,introStart:introStart,introEnd:introEnd,INTRO:INTRO,get cars(){return cars;},get ball(){return ball;},get player(){return player;},get phase(){return phase;},set phase(v){phase=v;},get scores(){return scores;},update:update,pads:pads,sdf:sdf,get timeLeft(){return timeLeft;},set timeLeft(v){timeLeft=v;},endReplay:endReplay,readPlayerInput:readPlayerInput,keys:keys,joy:joy,btn:btn,
  SV:()=>SV,go:go,toMenu:toMenu,MODE:MODE,MUT:MUT,HS:HS,DROP:DROP,HOOP:HOOP,TR:TR,MV:MV,MOVES:MOVES,mvLoad:mvLoad,mvBeginTry:mvBeginTry,mvSkip:mvSkip,IT:IT,LST:LST,owns:owns,buyItem:buyItem,openCrate:openCrate,doPlay:doPlay,lobby:lobby,configureMode:configureMode,matchRewards:matchRewards,givePower:givePower,PWK:PWK,
  get arena(){return {HX:HX,HZ:HZ,kind:ARN.kind};}};
})();
</script>
</body>
</html>
)rawliteral";
