import { Component } from '@angular/core';

import { SocketService } from "./bible.service";

@Component({
    selector: 'app-root',
    templateUrl: './app.component.html',
    styleUrls: ['./app.component.scss']
})
export class AppComponent {
    debug: boolean = true;

    constructor(
        private socketService: SocketService,
    ) {
        this.initIoConnection();
    }

    private initIoConnection(): void {
        this.socketService.initSocket();

        this.socketService.onEvent(this.socketService.event.CONNECT)
            .subscribe(() => {
                if (this.debug) {
                    console.debug("websocket connected");
                }
            });

        this.socketService.onEvent(this.socketService.event.DISCONNECT)
            .subscribe(() => {
                console.error("websocket disconnected");
            });
    }

}
