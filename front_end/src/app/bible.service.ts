import { Injectable } from '@angular/core';

import { HttpClient } from "@angular/common/http";

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
