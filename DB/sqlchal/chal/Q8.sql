SELECT subquery1.PN, subquery1.BPC, ROUND((subquery1.BPC * 1.0) / subquery2.TOTAL * 100, 1)
FROM (SELECT Books.publisher_id AS BPID, COUNT(Books.publisher_id) AS BPC, Publishers.name AS PN FROM BorrowedBooks AS BB, Books, Publishers
WHERE BB.borrow_date >= "2021-09-10" AND BB.borrow_date <= "2022-09-10"
AND BB.book_id = Books.book_id AND Books.publisher_id = Publishers.publisher_id
GROUP BY Books.publisher_id) AS subquery1, (SELECT COUNT(Books.publisher_id) AS TOTAL FROM BorrowedBooks AS BB, Books, Publishers
WHERE BB.book_id = Books.book_id AND Books.publisher_id = Publishers.publisher_id
AND BB.borrow_date >= "2021-09-10" AND BB.borrow_date <= "2022-09-10") AS subquery2
ORDER BY subquery1.BPC DESC, subquery1.PN ASC;