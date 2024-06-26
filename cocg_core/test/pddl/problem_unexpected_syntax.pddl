(define (problem simple_1)
  (:domain simple)
  (:objects
    leia - robot
    Jack - person
    kitchen bedroom - room
    m1 - message
  )
  (:init
    (robot_at leia kitchen)
    (person_at Jack bedroom)
    (= (room_distance kitchen bedroom) 10)

    ;; problem syntax below
    ;; too many rooms specified
    (is_teleporter_destination kitchen bedroom)
    ;; robot erroneously specified
    (= (room_distance leia kitchen) 10)

  )

  ;; The goal is to have both packages delivered to their destinations:
  (:goal
    (and
      (robot_talk leia m1 Jack)
    )
  )
)