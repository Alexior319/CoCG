(define (problem CM_R2_C2)
    (:domain coffee_serving)

    (:objects
        r1 r2 - robot
        ps p1 p2 pg - location
        c1 c2 - coffee
    )

    (:init
        (robot_at r1 ps)
        (hand_empty r1)
        (robot_at r2 ps)
        (hand_empty r2)
        (unknown
            (coffee_at c1 p1))
        (unknown
            (coffee_at c1 p2))

        (oneof
            (coffee_at c1 p1)
            (coffee_at c1 p2)
        )
        (unknown
            (coffee_at c2 p1))
        (unknown
            (coffee_at c2 p2))

        (oneof
            (coffee_at c2 p1)
            (coffee_at c2 p2)
        )
    )
    (:goal
        (and
            (coffee_at c1 pg)
            (coffee_at c2 pg)
        )
    )
)