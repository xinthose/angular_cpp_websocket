# Xinthose Installation

## Visual Studio Code

- Install Github extension
- Set your Visual Studio Code Personal Access Token
- Test commit and push

## Linux

- `cd /home/adam/Documents/svn/github/personal-website`
- `apt-get install nodejs npm build-essential`

## Angular 8

### Installation

- `npm i -g @angular/cli @angular/core @angular/forms @angular/forms terser typescript`

### Create App (client directory)

- `ng new personal-website --style=scss --enableIvy=false --routing=true`
  
#### Components

- `for i in home bible about page-not-found; do ng g c "${i}"; done`

#### Services

- `ng g service bible`

## General

- `npm i -g @angular/cli`
- `npm i rxjs rxjs-compat @angular/http`

## Kendo-UI (Progress)

- `ng add @progress/kendo-angular-buttons @progress/kendo-angular-dropdowns @progress/kendo-data-query`
- `npm i @progress/kendo-theme-material`

## MDBootstrap Pro

- tutorial: <https://mdbootstrap.com/angular/5min-quickstart/>
  - click "PRO" underneath "npm iation"
- `npm i git+https://oauth2:XXXXXXXXXX_XXXXXXXXX@git.mdbootstrap.com/mdb/angular/ng-uikit-pro-standard.git --save`
- `npm i chart.js@2.5.0 @types/chart.js easy-pie-chart@2.1.7 hammerjs@2.0.8 screenfull@3.3.0 @fortawesome/fontawesome-free animate.css`
  - 3rd party libraries

## References

- Bible in JSON: <https://github.com/thiagobodruk/bible>

## Commands -------------------------------------------------------------------------------------------------------------------------

## Angular Commands (ng)

- `ng --version`    // show angular version
- `ng update --all --force` // update all angular packages (ignore peer dependency errors)
- `ng build --prod --aot`   // build for production
- `ng serve --host 0.0.0.0` // allow outside PC's access to the app

## Kendo Node.js commands

- `ncu -u -f /^@progress/`  // check progress widget versions

## Node Commands

- `node -v` // show node version

## NPM Commands (Node Package Manager)

- `npm -v`  // show npm version
- `npm i -g npm`  // update npm version
- `npm update`  # update all packages in the app
- npm i [package]@[version]   // install specific version of a package
- `npm ls @progress/kendo-angular-scheduler`  // show version of a package
- `npm i --package-lock-only` // only create `package-lock.json` file

### Install dependencies

- `npm i -g npm-install-peers`
- `npm-install-peers`
- `npm i @angular/cli@latest`  // install latest version of angular cli

## Typescript Commands

- `tsc -v`  // show typescript version

## Github

- `git config --global user.email "your github login email address"`  // set email address
- `git config --global user.email`  // check email address

## pm2

- Github: <https://github.com/Unitech/pm2>
- `npm i pm2 pm2-logrotate -g`
- `pm2 ls`  // list all running applications
- `pm2 stop EDS4WSserver`
- `pm2 restart EDS4WSserver`
- `pm2 delete EDS4WSserver`
- `pm2 describe EDS4WSserver`
- `pm2 monit`   // monitor logs, custom metrics, & application information

## link pm2 to <https://app.pm2.io/#/bucket/5beae26a3ca4a1db69a9056b/overview/servers>

- `pm2 link 90n1bqb6mdqm111 mvp52pu0qi8k4v1 EDS`

### Update pm2

- `npm i pm2@latest -g`
- `pm2 update`

## Other

- `http-server`

## Notes

- `[style.height.%]="100"`  // full height grid (as long as content)

## Troubleshooting

- `ERROR in The Angular Compiler requires TypeScript >=3.4.0 and <3.5.0 but 3.5.1 was found instead.`
  - `npm i typescript@">=3.4.0 <3.5.0"`
