#!/usr/bin/env python3
"""Programmtests fuer Lernstandscheck I. Geheimcode fuer diese Tests: 427."""
from pathlib import Path
import re,subprocess,sys,tempfile,time,os

# Eingaben entsprechen der Menuebedienung, eine Zahl pro Zeile.
# Erwartete Werte folgen ausschliesslich den Spielregeln, nicht internen Funktionen.
FAELLE=[
 ('Abbruch','0\n',{},['BEENDET']),
 ('Status bleibt kostenlos','3\n3\n0\n',{'Versuche':[8,8],'Tipps':[0,0]},['BEENDET']),
 ('Zwei richtige Stellen','1\n417\n3\n0\n',{'Treffer':[2],'Versuche':[7],'Tipps':[1]},['BEENDET']),
 ('Eine richtige Stelle','1\n472\n0\n',{'Treffer':[1]},['BEENDET']),
 ('Vertauschte Ziffern','1\n724\n0\n',{'Treffer':[1]},['BEENDET']),
 ('Keine richtige Stelle','1\n111\n0\n',{'Treffer':[0]},['BEENDET']),
 ('Sofortiger Sieg','1\n427\n3\n2\n0\n',{'Treffer':[3]},['GEWONNEN']),
 ('Wiederholter Hinweis kostet erneut','2\n2\n3\n0\n',{'Hinweis':[13,13],'Versuche':[6],'Tipps':[0]},['BEENDET']),
 ('100 und 999 sind gueltig','1\n100\n1\n999\n3\n0\n',{'Treffer':[0,0],'Versuche':[6],'Tipps':[2]},['BEENDET']),
 ('99 und 1000 sind ungueltig','1\n99\n1\n1000\n3\n0\n',{'Versuche':[8],'Tipps':[0],'UNGUELTIG':2},['BEENDET']),
 ('Negativer Code','1\n-1\n3\n0\n',{'Versuche':[8],'Tipps':[0],'UNGUELTIG':1},['BEENDET']),
 ('Unbekannte Menueauswahl','4\n-1\n3\n0\n',{'Versuche':[8],'Tipps':[0],'UNGUELTIG':2},['BEENDET']),
 ('Gemischter Ablauf','1\n99\n2\n3\n1\n417\n3\n0\n',{'Treffer':[2],'Hinweis':[13],'Versuche':[7,6],'Tipps':[0,1],'UNGUELTIG':1},['BEENDET']),
 ('Sieg nach sieben Hinweisen','2\n'*7+'1\n427\n3\n0\n',{'Hinweis':[13]*7,'Treffer':[3]},['GEWONNEN']),
 ('Sieg nach sieben falschen Tipps','1\n100\n'*7+'1\n427\n3\n0\n',{'Treffer':[0]*7+[3]},['GEWONNEN']),
 ('Acht falsche Tipps sperren','1\n100\n'*8+'3\n1\n427\n0\n',{'Treffer':[0]*8},['GESPERRT']),
 ('Acht Hinweise sperren','2\n'*8+'3\n1\n427\n0\n',{'Hinweis':[13]*8},['GESPERRT']),
 ('Hinweis im letzten Versuch','1\n100\n'*7+'2\n3\n0\n',{'Treffer':[0]*7,'Hinweis':[13]},['GESPERRT']),
 ('Text statt Menuezahl beendet das Programm','abc\n3\n0\n',{'UNGUELTIG':1,'Exitstatus':1},[]),
 ('Text statt Code beendet das Programm','1\nabc\n417\n3\n0\n',{'UNGUELTIG':1,'Exitstatus':1},[]),
]
ZAHLEN=('Treffer','Hinweis','Versuche','Tipps')
ENDEN=('GEWONNEN','GESPERRT','BEENDET')

def ausfuehren(programm,eingaben):
    # Grenzen verhindern unbegrenzt laufende oder druckende Programme.
    with tempfile.TemporaryFile() as eingang, tempfile.TemporaryFile() as ausgang, tempfile.TemporaryFile() as fehler:
        eingang.write(eingaben.encode('utf-8')); eingang.seek(0)
        prozess=subprocess.Popen([str(programm)],stdin=eingang,stdout=ausgang,stderr=fehler)
        grenze=time.monotonic()+2
        try:
            while prozess.poll() is None:
                if time.monotonic()>grenze:
                    raise subprocess.TimeoutExpired(str(programm),2)
                if os.fstat(ausgang.fileno()).st_size+os.fstat(fehler.fileno()).st_size>1024*1024:
                    raise RuntimeError('Zu viel Ausgabe; Endlosschleife oder Diagnoseausgaben pruefen.')
                time.sleep(0.01)
            ausgang.seek(0); fehler.seek(0)
            return subprocess.CompletedProcess([str(programm)],prozess.returncode,
                ausgang.read(1024*1024).decode('utf-8',errors='replace'),
                fehler.read(1024*1024).decode('utf-8',errors='replace'))
        finally:
            if prozess.poll() is None:
                prozess.kill(); prozess.wait()

def pruefe(programm):
    fehlgeschlagen=0
    for name,eingaben,erwartet,enden in FAELLE:
        probleme=[]
        try:
            lauf=ausfuehren(programm,eingaben)
        except subprocess.TimeoutExpired:
            print(f'FEHLER: {name}: Programm endet nicht innerhalb von 2 Sekunden.')
            fehlgeschlagen+=1
            continue
        except RuntimeError as fehler:
            print(f'FEHLER: {name}: {fehler}'); fehlgeschlagen+=1
            continue
        except OSError as fehler:
            print(f'Programm kann nicht gestartet werden: {fehler}')
            return 1
        if lauf.returncode!=erwartet.get('Exitstatus',0):
            probleme.append(f'Exitstatus: erwartet {erwartet.get("Exitstatus",0)}, erhalten {lauf.returncode}')
        for label in ZAHLEN:
            ist=[int(n) for n in re.findall(r'\b'+label+r':[ \t]*([+-]?\d+)(?![\d.])',lauf.stdout)]
            soll=erwartet.get(label,[])
            if ist!=soll: probleme.append(f'{label}: erwartet {soll}, erhalten {ist}')
        ungueltig=len(re.findall(r'\bUNGUELTIG\b',lauf.stdout))
        if ungueltig!=erwartet.get('UNGUELTIG',0):
            probleme.append(f'UNGUELTIG: erwartet {erwartet.get("UNGUELTIG",0)} Meldungen, erhalten {ungueltig}')
        for ende in ENDEN:
            n=len(re.findall(r'\b'+ende+r'\b',lauf.stdout))
            soll=enden.count(ende)
            if n!=soll:probleme.append(f'{ende}: erwartet {soll} Meldungen, erhalten {n}')
        if probleme:
            fehlgeschlagen+=1
            print(f'FEHLER: {name}')
            for p in probleme:print('  '+p)
            print('  Eingaben: '+eingaben.replace('\n',' / '))
        else:print(f'OK: {name}')
    print(f'\n{len(FAELLE)-fehlgeschlagen}/{len(FAELLE)} Programmtests bestanden.')
    if fehlgeschlagen:
        print('Geheimcode 427, Ergebniszeilen, Spielregeln und erneutes Uebersetzen pruefen.')
    return 1 if fehlgeschlagen else 0

if __name__=='__main__':
    programm=Path(sys.argv[1]) if len(sys.argv)>1 else Path(__file__).with_name('tresor')
    if not programm.exists() and programm.with_suffix('.exe').exists():programm=programm.with_suffix('.exe')
    if not programm.is_file():
        print('Ausfuehrbare Datei fehlt. Zuerst make ausfuehren oder einen Programmpfad angeben.')
        sys.exit(1)
    sys.exit(pruefe(programm.resolve()))
