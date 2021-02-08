#include <amxmodx>
#include <amxmisc>
#include <http2>

public smsMainMenu(id){
	
	if( !playerLogged(id) ){
		ColorChat(id, GREEN, "%s Zaloguj sie aby moc korzystac z tego menu!", PREFIXSAY);
		return PLUGIN_HANDLED
	}
	
	new szName[33]
	get_user_name(id, szName, sizeof(szName))
	
	new gText[512]
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "Witaj w sklepie\r %s", szName)
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^nAby kupowac w sklepie musisz pierw doladowac swoje Luzaczki.")
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^nLuzaczki mozesz jeszcze zdobyc z\y /nagroda.\r Powodzenia!")
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\y1.\w Doladuj swoje\r Luzaczki\w !")		
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y2.\w Kupuj za\r Luzaczki\w !")	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y3.\w Twoje Uslugi\w !")
	
	if( isSuperAdmin(id)){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\y6.\w Dodaj\r Waluty\w!^n")
	}
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y0.\w Wyjscie")	
	show_menu(id, B1 | B2 | B3 | B6 | B0  , gText, -1, "smsMainMenu"  )
	return PLUGIN_HANDLED;
}
public smsMainMenu_2(id, item){
	/*new x = true;
	if(x){
		ColorChat(id, GREEN, "== Sklep zostal wylaczony ==")
		return PLUGIN_CONTINUE
	}*/
	switch(item){
		case 0:{/*
			
			if( !has_flag(id, HEADFLAG) ){
				ColorChat(id, GREEN, "== Sklep zostal wylaczony ==")
				return PLUGIN_CONTINUE
			}*/
			addCash(id)
		}
		case 1:{
			luzaczkiShop(id)
		}
		case 2:{
			yourBuyService(id)	
		}
		case 5:{

			if(!isSuperAdmin(id)){
				smsMainMenu(id)
				return PLUGIN_CONTINUE
			}
			addValueMenu(id)
		}
		
	}
	return PLUGIN_CONTINUE
}
public addValueMenu(id){
	new menu = menu_create("\r[BaseBuilder]\y Dodawanie Walut", "addValueMenu_2")	
	
	menu_additem(menu, "Dodaj VIP'a");
	menu_additem(menu, "Dodaj Luzaczki");
	menu_additem(menu, "Dodaj Brylki");
	menu_additem(menu, "Dodaj Lvl");
	menu_additem(menu, "Dodaj Czas");
	menu_additem(menu, "Dodaj Reset");
	menu_additem(menu, "Dodaj EXP'a");
	menu_additem(menu, "\dZresetuj nagrode");
	//menu_additem(menu, "\dZresetuj stamine");
	menu_additem(menu, "Dodaj Kosci");
	menu_additem(menu, "Skrzynki");
	
	menu_display(id, menu, 0);
}

public addValueMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu)
		return PLUGIN_CONTINUE;
	}
	
	if(!isSuperAdmin(id)){
		return PLUGIN_CONTINUE
	}
	
	switch(item){
		case 0:{
			userMenuPlayer[id] = MENU_GIVING_VIP
			choosePlayer(id, 0)
		}
		case 1:{
			userMenuPlayer[id] = MENU_GIVING_LUZCOIN
			choosePlayer(id, 0)
		}
		case 2:{
			userMenuPlayer[id] = MENU_GIVING_NUGGET
			choosePlayer(id, 0)
		}
		case 3:{
			userMenuPlayer[id] = MENU_GIVING_LVL
			choosePlayer(id, 0)
		}
		case 4:{
			userMenuPlayer[id] = MENU_GIVING_TIME
			choosePlayer(id, 0)
		}
		case 5:{
			userMenuPlayer[id] = MENU_GIVING_RESET
			choosePlayer(id, 0)
		}
		case 6:{
			userMenuPlayer[id] = MENU_GIVING_EXP
			choosePlayer(id, 0)
		}
		case 7:{
			userMenuPlayer[id] = MENU_PLAYER_AWARD
			choosePlayer(id, 0)
		}
		/*case 8:{
			userMenuPlayer[id] = MENU_PLAYER_STAMINA
			choosePlayer(id, 0)
		}*/
		case 8:{
			userMenuPlayer[id] = MENU_GIVING_BONES
			choosePlayer(id, 0)
		}
		case 9:{
			menuCreateCase(id);
		}
	}
	return PLUGIN_CONTINUE;
}
public addCash(id){
	new gText[128]
	format(gText, sizeof(gText), "Ile chcesz doladowac\r Luzaczkow?^n\dPosiadasz\r %d\d Luzaczkow", userLuzCoin[id]);
	new menu = menu_create(gText, "addCash_2")
	for(new i = 0; i < sizeof(pricesMenu) ; i ++ ){
		
		format(gText, sizeof(gText), "\y%s\r Luzaczkow\d [\w %s PLN\d | SMS ]", pricesMenu[i][4], pricesMenu[i][2] )
		menu_additem(menu, gText)
	}
	menu_setprop(menu,MPROP_NEXTNAME,"Dalej");
	menu_setprop(menu,MPROP_BACKNAME,"Wroc");
	menu_setprop(menu,MPROP_EXITNAME,"Wyjscie");
	
	menu_display(id, menu, 0)
}
public addCash_2(id, menu, item){
	if( item == MENU_EXIT ){
		smsMainMenu(id)
		return PLUGIN_HANDLED
	}
	userService[id] = item;
	displayBuy(id)
	return PLUGIN_HANDLED
}
public displayBuy(id){
	new item = userService[id] ;
	new gText[512]
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "SMS na doladowanie\y %s\r Luzaczkow", pricesMenu[item][4] )
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\wCena:\r %s PLN", pricesMenu[item][2] )
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\wWyslij SMS o tresci\r %s\w na numer\r %s\w", pricesMenu[item][0], pricesMenu[item][1] )
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^nUpewnij sie ze wpisales dobrze\r tresc\w i\r numer" )
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\n\y1.\w Wpisz kod zwrotny z\r SMS" )
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y2.\w Wroc" )
	show_menu(id, B1 | B2 , gText, -1, "displayBuy")
}
public displayBuy_2(id, item){
	switch(item){
		case 0:{
			cmd_execute(id, "messagemode KodZwrotny")
			displayBuy(id);
		}
		case 1:{
			addCash(id);
		}
	}
}
public yourBuyService(id){
	new szName[33]
	get_user_name(id, szName, sizeof(szName))
	
	new gText[512]
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dWitaj\r %s\d oto twoj czas uslug^n^n", szName)
	
	new daysLeft = (timeVip[id] - get_systime())	
	
	if( daysLeft > 0 || isVip(id) ){
		if( has_flag(id, flagVip)){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPozostalo\y [Bez Limitu Vip]^n")
		} else {
			iLen += format(gText[iLen], sizeof(gText)-iLen-1,"\dPozostalo\y [%d:%s%d:%s%d]\d Vipa^n",( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ))
		}
	} else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPozostalo\y [Brak Vipa]^n")
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r0.\w Wyjscie")	
	show_menu(id,  B0  , gText, -1, "yourBuyService"  )
	
}
public yourBuyService_2(id, item){
	switch(item){
		default:{
			return;
		}
	}
}

public addValue(id){
	if(!isSuperAdmin(id))
		return PLUGIN_CONTINUE;
	new szArg[9]
	read_argv(1, szArg, sizeof(szArg))
	remove_quotes(szArg)
	new target = userService[id]
	new value = str_to_num(szArg)
	
	
	new gText[128];
	logType[id] = LOG_ADD;
	if(logType[id] == LOG_ADD){
		
		switch(userMenuPlayer[id]){
				
			case MENU_GIVING_VIP:{
				
				if(has_flag(target, flagVip)){
					ColorChat(id, GREEN, "---^x01 Gracz:^x03 %s^x01 posiada flage^x03 't'^x01 i posiada^x01 VIP'a^x01 bez limitu^x04 ---", userName[target])
					choosePlayer(id, 0)
					return PLUGIN_CONTINUE;
				}
				
				timeVip[target] = max( timeVip[target] + (value*60), get_systime() + (value*60) )
				new daysLeft = (timeVip[target] - get_systime())	
				userVip[id ] = !!(daysLeft>0);
				
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d min VIP'a^x04][^x03Graczowi %s^x04] ---", value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d min VIP'a^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Vipa [%s] na [%d min]", userName[target], (daysLeft /60) )	
			} 
			case MENU_GIVING_LUZCOIN:{
				userLuzCoin[target] += value
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d Luzaczkow^x04][^x03Graczowi %s^x04] ---", value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d Luzaczkow^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Luzaczki [%s] ilosc [%d]", userName[target], value)	
		
			}
			case MENU_GIVING_NUGGET:{
				addNuggetToFinal(target, value)
				//userNugget[target] += value
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d Brylek^x04][^x03Graczowi %s^x04] ---", value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d Brylek^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Brylki [%s] ilosc [%d]", userName[target], value)	
			}
			case MENU_GIVING_LVL:{
				
				
				userLevel[target] += value
				userLevel[target] = clamp(userLevel[target], 0, MAXLVL);
	
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d Lv.^x04][^x03Graczowi %s^x04] ---",value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d Lv.^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Lv. [%s] ilosc [%d ]", userName[target], value)	
			}
			case MENU_GIVING_TIME:{
				
				
				userTime[target] += value *60
				
				//userTime[target] += value*60
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d min Czasu Gry^x04][^x03Graczowi %s^x04] ---", value, userName[target])				
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d min Czasu Gry^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Czas Gry [%s] na [%d min]", userName[target], value)
			}
			case MENU_GIVING_RESET:{
				userReset[target] += value
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d Reset^x04][^x03Graczowi %s^x04] ---", value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d Reset^x04][^x03Od Admina %s^x04] ---", value, userName[id])	
				format(gText, sizeof(gText), "dodal Reset [%s] ilosc [%d]", userName[target], value)
			}
			case MENU_GIVING_EXP:{
				
				addExpToFinal(target, float(value));
				//userExp[target] += float(value);
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d Xp.^x04][^x03Graczowi %s^x04] ---",value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d Xp.^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Xp. [%s] ilosc [%d ]", userName[target], value)
			}
			case MENU_GIVING_BONES:{
				userBone[target] += value
				ColorChat(id, GREEN, "---^x01 Dodales^x04 [^x03%d Kosci^x04][^x03Graczowi %s^x04] ---", value, userName[target])
				ColorChat(target, GREEN, "---^x01 Otrzymales^x04 [^x03%d Kosci^x04][^x03Od Admina %s^x04] ---", value, userName[id])
				format(gText, sizeof(gText), "dodal Kosci [%s] ilosc [%d]", userName[target], value)	
			}
		}
	}
	choosePlayer(id, 0)
	logBB(id, gText)
	return PLUGIN_CONTINUE;
}
public addValueTransfer(id){
	new szArg[9]
	read_argv(1, szArg, sizeof(szArg))
	remove_quotes(szArg)
	new target = userService[id]
	new value = str_to_num(szArg)
	
	if(!is_user_connected(target)){
		ColorChat(id ,GREEN, "---^x01 Nie ma gracza na serwerze ktoremu chciales dac Brylki^x04 ---");
		return PLUGIN_CONTINUE;
	}
	
		
	switch(userMenuPlayer[id]){	
		case MENU_PLAYER_NUGGET:{
				
			value = abs(str_to_num(szArg))
			if( value == 0){
				ColorChat(id, GREEN, "%s Nie mozesz dac 0 Brylek!", PREFIXSAY)
				return PLUGIN_CONTINUE
			}
			if( value > userNugget[id] ){
				ColorChat(id, GREEN, "%s Nie masz tyle Brylek!", PREFIXSAY)
				return PLUGIN_CONTINUE
			}
			new gText[128];
			logType[id] = LOG_TRANSFER;
			if(logType[id] == LOG_TRANSFER) format(gText, sizeof(gText), "wyslal [%d] brylek graczowi [%s]", value, userName[target])
			logBB(id, gText)
			
			
			userNugget[id]		-=	value
			userNugget[target]	+=	value
			ColorChat(id, 		GREEN, 	"%s Wyslales^x03 %s^x01 Brylek do gracza^x04 %s!", PREFIXSAY, formatNumber(value), userName[target] ) 
			ColorChat(target, 	GREEN, 	"%s Otrzymales^x03 %s^x01 Brylek od gracza^x04 %s!", PREFIXSAY, formatNumber(value), userName[id] ) 
		}
	}
	choosePlayer(id, 0)
	return PLUGIN_CONTINUE;
}

public codeCheck(id){
	if( get_gametime() - fCodeCheckTime < 5.0 ){		
		ColorChat(id, GREEN, "[SKLEP]^x01 Luzaczki wlasnie sprawdzaja juz kod, poczekaj chwile!")
		return PLUGIN_HANDLED;
	}
	fCodeCheckTime=get_gametime();
	read_argv(1, szSmsCode, sizeof(szSmsCode))
	remove_quotes(szSmsCode)
	
	lastBuyer=id;
	
	ColorChat(id, GREEN, "[SKLEP]^x01 Luzaczki sprawdzaja kod, prosze czekac...")
	new szLink[512]	
	format(szLink, sizeof(szLink), "http://cssetti.pl/Api/SmsApiV2CheckCode.php?UserId=%s&Code=%s", APISMS, szSmsCode);

	HTTP2_Download(szLink, _, _, "checkCodeSmsSeti");
	return PLUGIN_CONTINUE	
}



public checkCodeSmsSeti(idHandler, error){	
	
	new gText[128]
	
	if(!is_user_connected(lastBuyer)){
		
		new gText[128];
		logType[lastBuyer] = LOG_BUY;
		if(logType[lastBuyer] == LOG_BUY){
			format(gText, sizeof(gText), "wpisal kod SMS i nie ma go na serwerze!")
		}
		logBB(lastBuyer, gText)
		return PLUGIN_HANDLED;
	}
	
	fCodeCheckTime	=	0.0;
	new szData[64], iLen, szName[33];
	get_user_name(lastBuyer, szName, sizeof(szName));
	
	HTTP2_getData(szData, sizeof(szData)-1, iLen);
	log_amx("Odpowiedz : %s | %", szData, szName)
	
	if( equal(szData, "-3", 2)){
		cmd_execute(lastBuyer, "messagemode KodZwrotny");
		ColorChat(lastBuyer, GREEN, "[SKLEP]^x01 Kod SMS pusty! Sprobuj ponownie.");			
		return PLUGIN_HANDLED;
	}
	else if( equal(szData, "-2", 2) ){
		cmd_execute(lastBuyer, "messagemode KodZwrotny");
		ColorChat(lastBuyer, GREEN, "[SKLEP]^x01 Kod SMS posiada nieprawidlowe znaki! Sprobuj ponownie.");			
		return PLUGIN_HANDLED;
	}
	else if( equal(szData, "-1", 2) ){
		ColorChat(lastBuyer, GREEN, "[SKLEP]^x01 Luzaczkowy Sklep napotkal problemy, sprobuj ponownie pozniej.");
		log_amx("Sklep niepoprawny user ID")			
		return PLUGIN_HANDLED;
	}
	else if( equal(szData, "0", 1) ){
		cmd_execute(lastBuyer, "messagemode KodZwrotny");
		ColorChat(lastBuyer, GREEN, "[SKLEP]^x01 Kod SMS jest nieprawidlowy! Sprobuj ponownie.");
		return PLUGIN_HANDLED;
	}
	else {
		for( new i = sizeof(pricesMenu)-1 ; i >= 0 ; i -- ){
			if( str_to_float(pricesMenu[i][3]) == str_to_float(szData) ){
				ColorChat(lastBuyer, GREEN, "(:^x01 Doladowales^x04 %s^x01 Luzaczkow^x04 :)", pricesMenu[i][4])
				ColorChat(0, GREEN, "(:^x01 Dziekujemy!^x03 %s^x01 kupil^x03 Luzaczki^x04 :)", szName)
				userLuzCoin[lastBuyer] += str_to_num(pricesMenu[i][4]);
				format(gText, sizeof(gText), "doladowanie [%s PLN][%s Cena][Kod: %s][Ilosc: %s]", pricesMenu[i][3], pricesMenu[i][2], szSmsCode, pricesMenu[i][4] )
				
				logType[lastBuyer] = LOG_BUY;
				if(logType[lastBuyer] == LOG_BUY){
					format(gText, sizeof(gText), "doladowanie [%s PLN][%s Cena][Kod: %s][Ilosc: %s]", pricesMenu[i][3], pricesMenu[i][2], szSmsCode, pricesMenu[i][4] )
				}
				logBB(lastBuyer, gText)
				return PLUGIN_HANDLED;
			}
		}
	}
	return PLUGIN_HANDLED;
	
}
public luzaczkiShop(id){
	if( !playerLogged(id) ){
		ColorChat(id, GREEN, "%s Zaloguj sie aby moc korzystac z tego menu!", PREFIXSAY);
		return PLUGIN_HANDLED
	}
	
	new gText[128]
	new daysLeft = (timeVip[id] - get_systime())	
	
	
	
	if( daysLeft > 0 ||isVip(id)){
		if(has_flag(id, flagVip)){
			format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y [Bez Limitu Vip]", userLuzCoin[id])
		} else {
			format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y %d:%s%d:%s%d\d Vipa",userLuzCoin[id], ( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ))
		}
	} else  format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y [Brak Vipa]", userLuzCoin[id])

	
	new menu = menu_create(gText, "luzaczkiShop_2")
	
	//new iLen = 0;
	//iLen += format(gText[iLen], sizeof(gText)-iLen-1, "%s", itemsShop[0][0])
	//menu_additem(menu, gText)
	
	for( new i = 0; i < sizeof(itemsShop); i ++ ){
		format(gText, sizeof(gText), "%s\d [\y %s\r Luzaczkow\d ]", itemsShop[i][0], itemsShop[i][1])
		menu_additem(menu, gText)
	}
	
	menu_setprop(menu,MPROP_NEXTNAME,"Dalej");
	menu_setprop(menu,MPROP_BACKNAME,"Wroc");
	menu_setprop(menu,MPROP_EXITNAME,"Wyjscie");
	
	menu_display(id, menu, 0)
	return PLUGIN_CONTINUE;
}


new userBuyVipDay[33];
new bool:userBuyVipAccept[33];

public luzaczkiShop_2(id, menu, item){
	if( item == MENU_EXIT ){
		smsMainMenu(id)
		return PLUGIN_HANDLED
	}
	if( userLuzCoin[id] < str_to_num(itemsShop[item][1]) ){
		ColorChat(id, GREEN, "---^x01 Niestety nie masz na to^x03 Luzaczkow^x04 ---")
		luzaczkiShop(id)
		return PLUGIN_HANDLED
	}
	userService[id] = item;
	
	/*if(item == 0){
		userBuyVipDay[id] = 0;
		vipMenuBuy(id)
		return PLUGIN_CONTINUE
	}*/
	luzaczkiFinal(id)
	return PLUGIN_CONTINUE
}

public luzaczkiFinal(id){
	new item = userService[id] ;
	new gText[512]
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "Czy na pewno chcesz kupic:^n^n")
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPrzedmiot:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],itemsShop[item][0])
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dCena:\r %s Luzakow^n", symbolsCustom[SYMBOL_DR_ARROW],itemsShop[item][1])
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s",symbolsCustom[SYMBOL_DR_ARROW], itemsShop[item][2])
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\y1.\w Tak ! Biore !")
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y2.\r Nie ! Absolutenie !")
	
	show_menu(id, B1 | B2 | B3, gText, -1, "luzaczkiFinal")
}


public luzaczkiFinal_2(id, item){
	if( item != 0 ){
		luzaczkiShop(id)
		return PLUGIN_CONTINUE
	}
	if( !playerLogged(id) ){	
		ColorChat(id, GREEN, "[SKLEP]^x01 Zaloguj sie aby kupowac!")
		return PLUGIN_CONTINUE
	}
	
	item = userService[id];
	switch(item){
		case 0:{
			/*userBuyVipAccept[id] = false;
			userBuyVipDay[id] = 0;
			vipMenuBuy(id)
			return PLUGIN_CONTINUE;*/
			timeVip[id]	= 	max( timeVip[id] + (DAY*30), get_systime() + (DAY*30) )
		}		
		case 1:{
			new newLeftExp  =  userScrollExp[id] - playedTime(id);
			if(newLeftExp <= 0){
				userScrollExp[id] = playedTime(id)
				userScrollExp[id] += (HOUR*6)	
			} else userScrollExp[id] += (HOUR*6)		
	
		}
		case 2:{
			new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
			if(newLeftNugget <= 0){
				userScrollNugget[id] = playedTime(id)
				userScrollNugget[id] += (HOUR*6)		
			} else userScrollNugget[id] += (HOUR*6)		
			
		}
		case 3:{
			
			new newLeftMine  =  userBoostMine[id] - playedTime(id);
			if(newLeftMine <= 0){
				userBoostMine[id] = playedTime(id)
				userBoostMine[id] += (MINUTE*55)
			} else userBoostMine[id] += (MINUTE*55)
			
		}
	}
	userLuzCoin[id] -= str_to_num(itemsShop[item][1])
	ColorChat(id, GREEN, "---^x01 Kupiles^x03 %s^x01 za^x03 %s Luzaczkow^x04 ---", itemsShop[item][0], itemsShop[item][1] )
	Display_Fade(id, 4096,4096, 4096, 64, 255, 64, 80);
	set_dhudmessage(30, 160, 30, -1.0, 0.20, 0, 0.5, 2.0, 0.5, 0.5)
	show_dhudmessage(id, "Kupiles %s za %s Luzaczkow", itemsShop[item][0], itemsShop[item][1]);
	new gText[128];
	logType[id] = LOG_BUY;
	if(logType[id] == LOG_BUY){
		format(gText, sizeof(gText), "kupil [%s] za [%s Luzaczkow]", itemsShop[item][0], itemsShop[item][1] )	
	}
	logBB(id, gText)
	luzaczkiShop(id)
	
	return PLUGIN_HANDLED
	
}

new vipCostDay = 1
public vipMenuBuy(id){
	new x = false;
	
	if(!x){
		ColorChat(id, GREEN, "---^x01 Myslales, ze takim sposobem kupisz VIPA?^x04 ---");
		return;
	}
	
	new item = userService[id] ;
	new gText[512]
	new iLen;
	new daysLeft = (timeVip[id] - get_systime())	
	new gratis[33];

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\wPosiadasz\r %d\w Luzaczkow^n", userLuzCoin[id])
	
	
	format(gratis, sizeof(gratis), " + %d dni gratisu", userBuyVipDay[id] >= 180 ? 30 : userBuyVipDay[id] >= 150 ? 25 : userBuyVipDay[id] >= 120 ? 20 : userBuyVipDay[id] >= 90 ? 15 : userBuyVipDay[id] >= 60 ? 10 : 5)
	
	
	if( daysLeft > 0 || isVip(id) ){
		if( has_flag(id, flagVip)) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\wPozostalo\r Bez Limitu Vip^n")
		 else iLen += format(gText[iLen], sizeof(gText)-iLen-1,"\wPozostalo\r %d godz %s%d min %s%d sek\w VIP'a^n",( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ))
		
	
	
	}
	
	if(userBuyVipDay[id] > 0) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\wCzy na pewno chcesz kupic\r VIP'a\w na:\r %d dni%s^n^n", userBuyVipDay[id], userBuyVipDay[id] >= 30 ? gratis : "")
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], itemsShop[item][2])
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dTaryfikator Cenowy:\r 1 dzien za %d Luzaczkow^n", symbolsCustom[SYMBOL_DR_ARROW], vipCostDay)
	
	if(userBuyVipDay[id] == 0) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dCena:\r Wpisz najpierw ilosc Dni!^n^n", symbolsCustom[SYMBOL_DR_ARROW], userBuyVipDay[id] * vipCostDay)
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dCena:\r %d Luzakow^n^n", symbolsCustom[SYMBOL_DR_ARROW], userBuyVipDay[id] * vipCostDay)
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r1.\w Wpisz ilosc Dni^n")
	if(userBuyVipDay[id] == 0)
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d2. Kup VIP'a!^n")
	else  iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r2.\w Kup VIP'a! %s^n", userBuyVipAccept[id] ? "\r[POTWIERDZ]":"")
	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r3.\y Jednak nie kupuje")
	
	show_menu(id, B1 | B2 | B3, gText, -1, "vipMenuBuy")
	
}
public vipMenuBuy_2(id, item){
	
	new x = false;
	
	if(!x){
		ColorChat(id, GREEN, "---^x01 Myslales, ze takim sposobem kupisz VIPA?^x04 ---");
		return PLUGIN_CONTINUE;
	}
	
	switch(item){
		case 0:{
			userBuyVipAccept[id] = false;
			vipMenuBuy(id)
			client_print(id, print_center, "Teraz wpisz na ile dni chcesz kupic VIP'a");
			cmd_execute(id, "messagemode IloscDni")
		}
		case 1:{
			new costDay = userBuyVipDay[id] * vipCostDay
			if(userBuyVipDay[id] == 0 ){
				vipMenuBuy(id)
				return PLUGIN_HANDLED
			}
			if( userLuzCoin[id] < costDay){
				ColorChat(id, GREEN, "---^x01 Niestety nie masz na to^x03 Luzaczkow^x04 ---")
				vipMenuBuy(id)
				return PLUGIN_HANDLED
			}
		
			if(!userBuyVipAccept[id]){
				userBuyVipAccept[id] = true
				vipMenuBuy(id)
				return PLUGIN_CONTINUE;
			}
			
			
			
			userLuzCoin[id] -= 	costDay
			
			new bonus = 0;
			
			if(userBuyVipDay[id] >= 30 )	
				bonus = userBuyVipDay[id] >= 180 ? 30 : userBuyVipDay[id] >= 150 ? 25 : userBuyVipDay[id] >= 120 ? 20 : userBuyVipDay[id] >= 90 ? 15 : userBuyVipDay[id] >= 60 ? 10 : 5
		
			timeVip[id]	= 	max( timeVip[id] + (DAY*(userBuyVipDay[id]+ bonus)), get_systime() + (DAY*(userBuyVipDay[id]+ bonus)) )
			ColorChat(id, GREEN, "---^x01 Kupiles^x03 VIP'a^x01 na^x03 (%d Dni)^x01 za^x03 %d Luzaczkow^x04 ---", userBuyVipDay[id]+bonus, costDay)
			Display_Fade(id, 4096,4096, 4096, 64, 255, 64, 80);
			set_dhudmessage(30, 160, 30, -1.0, 0.20, 0, 0.5, 2.0, 0.5, 0.5)
			show_dhudmessage(id, "Kupiles VIP'a na (%d Dni) za %d Luzaczkow", userBuyVipDay[id]+bonus, costDay);
			new gText[128];
			logType[id] = LOG_BUY;
			if(logType[id] == LOG_BUY){
				format(gText, sizeof(gText), "kupil [VIP'a na (%d Dni)] za [%d Luzaczkow]", userBuyVipDay[id]+bonus, costDay)	
			}
			logBB(id, gText)
			
			userBuyVipAccept[id] = false;
			
			luzaczkiShop(id)
		}
		case 2:{
			return PLUGIN_HANDLED;
		}
		
	}
	return PLUGIN_CONTINUE;
		
}

public buyVipDay(id){
	
	new x = false;
	
	if(!x){
		ColorChat(id, GREEN, "---^x01 Myslales, ze takim sposobem kupisz VIPA?^x04 ---");
		return;
	}
	
	new szArg[9]
	read_argv(1, szArg, sizeof(szArg))
	remove_quotes(szArg)
	new value = abs(str_to_num(szArg))
	
	userBuyVipDay[id] = value
	
	vipMenuBuy(id)
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
