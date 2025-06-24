#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import re

def main():
    """
    ეს ფილტრი კითხულობს Markdown ფაილს stdin-დან და
    ` ```mermaid ` ბლოკებს გარდაქმნის `@startuml` ... `@enduml` ბლოკებად,
    რათა Doxygen-მა PlantUML-ით დაამუშაოს.
    """
    try:
        content = sys.stdin.read()
        
        # SonarQube (S5361) რეკომენდაცია: ვიყენებთ str.replace()-ს re.sub()-ის ნაცვლად,
        # რადგან ვცვლით ფიქსირებულ სტრიქონს. ეს უფრო სწრაფი და უსაფრთხოა (იცავს ReDoS შეტევისგან).
        content = content.replace('```mermaid', '@startuml')
        
        # გამოვიყენოთ მარტივი state machine, რათა დავხუროთ მხოლოდ ის ` ``` ` ბლოკები,
        # რომლებიც @startuml-ის შემდეგ მოდის.
        output_lines = []
        in_uml_block = False
        for line in content.splitlines():
            # ზუსტი დამთხვევა, რათა არ მოხდეს შეცდომა სხვა კოდის ბლოკებთან
            if line.strip() == '@startuml':
                in_uml_block = True
                output_lines.append(line)
            elif line.strip() == '```' and in_uml_block:
                in_uml_block = False
                output_lines.append('@enduml')
            else:
                output_lines.append(line)
        
        # დავბეჭდოთ შედეგი stdout-ში, რასაც Doxygen-ი წაიკითხავს
        print('\n'.join(output_lines))

    except Exception as e:
        # შეცდომის შემთხვევაში, დავბეჭდოთ stderr-ში, რომ Doxygen-მა არ დაამუშაოს
        print(f"Filter error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()