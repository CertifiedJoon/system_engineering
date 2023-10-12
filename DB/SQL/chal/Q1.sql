SELECT Books.title, Authors.name, Members.name, BB.borrow_date FROM Borrowedbooks as BB, Books, Authors, Members
WHERE BB.book_id = Books.book_id AND Books.author_id = Authors.author_id AND BB.member_id = Members.member_id
ORDER BY BB.borrow_date DESC, BB.borrowed_id
LIMIT 3 OFFSET 3;