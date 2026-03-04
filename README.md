no markdown format, because I'm lazy.. zzz

IMEkey is a utility for force-assign en-US keyboard layout when system idle 3000mS, en-US layout and 3000mS is set able, just no configure file now, because I'm lazy..

IMEkey for windows now, X11 version in work.

command: 
    -time=<value> : system idle timeout,mS, ex: -time=3000 mean active when after system idle 3000mS.
    -lang=<hex_value> : language ID, ex -lang=0x409 mean reset keyboard layout to en-US when active.

printf/qdebug can not show message in Qt now.

update for chinese:
這是一個輸入法強制切換器, 目前僅給 windows 用, linux/x11 的我還在研究. 簡單的說, 它會在鍵盤滑鼠等全部都不動過約 3秒鐘 (時間可以設定) 後, 就會把輸入法切回英文. 這樣做有個前提是, 你 windows 語系要有安裝 en-US, 否則也會以現有輸入法的英數輸入為準, 如果都找不到, 它會在 lang 裡寫 not found. 致於為什麼會寫這東西? 因為我不想在做其它事時被輸入法干擾到.
