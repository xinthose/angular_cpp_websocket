import { Component, OnInit } from '@angular/core';

import * as website_package from "../../../package.json";

@Component({
  selector: 'app-about',
  templateUrl: './about.component.html',
  styleUrls: ['./about.component.scss']
})
export class AboutComponent {

  websiteVersion: string;

  constructor() {
    this.websiteVersion = website_package.version;
  }


}
