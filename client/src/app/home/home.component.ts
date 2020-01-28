import { Component, OnInit } from '@angular/core';

import { SocketService } from "../bible.service";

interface flowUpdate {
  flowAcc: number,
  flowRate: number,
  flowPH: number,
  flowConductivity: number,
  highLevel: boolean,
  sampleInProgress: boolean,
}

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.scss']
})
export class HomeComponent implements OnInit {

  constructor(
    private socketService: SocketService,
  ) {
    // subscribe to websockets
    this.socketService.onEvent(this.socketService.event.STATION_CLOSED)
      .subscribe(() => {
        console.debug("station closed");
      });

    // send an event to C++
    let data = {
      "product_type": 1,
      "product_type_sub_type": 2,
      "requested_volume": 100,
      "manifest_number": "",
    }
    this.socketService.sendEvent(this.socketService.event.TRANS_STARTED, data);

    // receive an event with data from C++
    this.socketService.onFlowUpdated()
      .subscribe((data: flowUpdate) => {
        console.debug("flow updated >> data = %O", data);
      });

  }

  ngOnInit() {
  }

}
