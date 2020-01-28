import { Injectable } from '@angular/core';

import { HttpClient } from "@angular/common/http";

// rxjs
import { Observable } from "rxjs";

// Other
import * as socketIo from 'socket.io-client';

@Injectable({
  providedIn: 'root'
})
export class BibleService {

  constructor(private http: HttpClient) { }

  public fetch(file: string) {
    return this.http.get(file);
  }
  public fetchBooks() {
    return this.http.get<Array<any>>('./assets/bible/books.json');
  }
  public fetchChapters() {
    return this.http.get<Array<{ chapterName: string, chapterId: number, bookId: number }>>('./assets/bible/chapters.json');
  }
  public fetchVerses() {
    return this.http.get<Array<{ verseName: string, verseId: number, chapterId: number, bookId: number }>>('./assets/bible/verses.json');
  }
}

/* #endregion */
/******************************** Web Sockets ********************************/
/* #region */

export enum Event {
  CONNECT = "connect",
  DISCONNECT = "disconnect",
  STATION_CLOSED = "station_closed",
  STATION_OPEN = "station_open",
  NEW_SCREEN = "new_screen",
  TRANS_STARTED = "trans_started",
  USER_LOGGED_IN = "user_logged_in",
  USER_LOGGED_OUT = "user_logged_out",
  FLOW_UPDATED = "flow_updated",
};

interface flowUpdate {
  flowAcc: number,
  flowRate: number,
  flowPH: number,
  flowConductivity: number,
  highLevel: boolean,
  sampleInProgress: boolean,
}

@Injectable({
  providedIn: "root"
})
export class SocketService {
  private debug: boolean = true;
  private BASE_URL: string;
  private socket;
  public event = Event;

  constructor(
  ) {
    //this.BASE_URL = SERVER + ":8090/";
    this.BASE_URL = "http://localhost:8090/";
  }

  public initSocket(): void {
    if (this.debug) {
      console.debug("initialize websocket at " + this.BASE_URL);
    }
    this.socket = socketIo(this.BASE_URL);
  }

  public closeSocket(): void {
    this.socket.close();
  }

  public sendEvent(event: Event, data?: Object): void {
    if (this.debug) {
      console.debug("sendEvent >> event = %s; data = %o", event, data);
    }

    this.socket.emit(event.toString(), data);
  }

  public onEvent(event: Event): Observable<Event> {
    return new Observable<Event>(observer => {
      this.socket.on(event, () => observer.next());
    });
  }

  public onFlowUpdated(): Observable<flowUpdate> {
    return new Observable<flowUpdate>(observer => {
      this.socket.on("flow_updated", (data: flowUpdate) => observer.next(data));
    });
  }
}

/* #endregion */
