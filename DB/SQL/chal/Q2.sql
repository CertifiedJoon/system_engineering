SELECT Members.member_id, Members.name, Borrow.BC FROM (SELECT BB.member_id, COUNT(BB.member_id) AS BC FROM BorrowedBooks AS BB
WHERE BB.borrow_date >= "2022-07-01" AND BB.borrow_date < "2022-08-01"
GROUP BY BB.member_id) AS Borrow, Members
WHERE Borrow.BC > 1 AND Borrow.member_id = Members.member_id
ORDER BY Members.member_id;